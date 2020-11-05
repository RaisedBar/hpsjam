/*-
 * Copyright (c) 2020 Hans Petter Selasky. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "hpsjam.h"

#include "peer.h"

#include "clientdlg.h"

#include "timer.h"

#include <QApplication>
#include <QMessageBox>
#include <QMutex>

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <err.h>

unsigned hpsjam_num_server_peers;
uint64_t hpsjam_server_passwd;
class hpsjam_server_peer *hpsjam_server_peers;
class hpsjam_client_peer *hpsjam_client_peer;
struct hpsjam_socket_address hpsjam_v4;
struct hpsjam_socket_address hpsjam_v6;
class QMutex *hpsjam_locks;

static const struct option hpsjam_opts[] = {
	{ "NSDocumentRevisionsDebugMode", required_argument, NULL, ' ' },
	{ "port", required_argument, NULL, 'p' },
	{ "server", no_argument, NULL, 's' },
	{ "peers", required_argument, NULL, 'P' },
	{ "password", required_argument, NULL, 'K' },
	{ "daemon", no_argument, NULL, 'B' },
	{ "jacknoconnect", no_argument, NULL, 'J' },
	{ "jackname", required_argument, NULL, 'n' },
	{ NULL, 0, NULL, 0 }
};

static void
usage(void)
{
        fprintf(stderr, "HpsJam [--server --peers <1..256>] [--port " HPSJAM_DEFAULT_PORT_STR "] "
		"[--daemon] [--password <64_bit_hex_password>] \\\n"
		"	[--jacknoconnect] [--jackname <name>]\n");
        exit(1);
}

Q_DECL_EXPORT int
main(int argc, char **argv)
{
	int c;
	int port = HPSJAM_DEFAULT_PORT;
	int do_fork = 0;
	bool jackconnect = true;
	const char *jackname = "hpsjam";

	QApplication app(argc, argv);

	/* set consistent double click interval */
	app.setDoubleClickInterval(250);

	while ((c = getopt_long_only(argc, argv, "p:sP:hBJ:n:K:", hpsjam_opts, NULL)) != -1) {
		switch (c) {
		case 's':
			if (hpsjam_num_server_peers == 0)
				hpsjam_num_server_peers = 1;
			break;
		case 'p':
			port = atoi(optarg);
			if (port <= 0 || port >= 65536)
				usage();
			break;
		case 'P':
			if (hpsjam_num_server_peers == 0)
				usage();
			hpsjam_num_server_peers = atoi(optarg);
			if (hpsjam_num_server_peers == 0 || hpsjam_num_server_peers > HPSJAM_PEERS_MAX)
				usage();
			break;
		case 'B':
			do_fork = 1;
			break;
		case 'J':
			jackconnect = false;
			break;
		case 'n':
			jackname = optarg;
			break;
		case 'K':
			if (sscanf(optarg, "%llx", (long long *)&hpsjam_server_passwd) != 1)
				usage();
			break;
		case ' ':
			/* ignore */
			break;
		default:
			usage();
			break;
		}
	}

	if (do_fork && daemon(0, 0) != 0)
		errx(1, "Cannot daemonize");

	if (hpsjam_num_server_peers == 0) {
		hpsjam_client_peer = new class hpsjam_client_peer;
		hpsjam_locks = new class QMutex [1];
		auto client = new HpsJamClient();

#ifdef HAVE_JACK_AUDIO
		if (hpsjam_sound_init(jackname, jackconnect)) {
			QMessageBox::information(client, QObject::tr("NO AUDIO"),
				QObject::tr("Cannot connect to JACK server or \n"
					    "sample rate is different from %1Hz or \n"
					    "latency is too high").arg(HPSJAM_SAMPLE_RATE));
		}
#endif
		client->show();
	} else {
		hpsjam_server_peers = new class hpsjam_server_peer [hpsjam_num_server_peers];
		hpsjam_locks = new class QMutex [hpsjam_num_server_peers];
	}

	/* create sockets, if any */
	hpsjam_socket_init(port);

	/* create timer, if any */
	hpsjam_timer_init();

	return (app.exec());
}
