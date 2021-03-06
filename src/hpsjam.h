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

#ifndef	_HPSJAM_H_
#define	_HPSJAM_H_

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

#define	HPSJAM_SAMPLE_RATE 48000
#define	HPSJAM_DEF_SAMPLES (HPSJAM_SAMPLE_RATE / 1000)
#define	HPSJAM_NOM_SAMPLES ((3 * HPSJAM_SAMPLE_RATE) / (2 * 1000))
#define	HPSJAM_WINDOW_TITLE "HPS Online Jamming"
#define	HPSJAM_VERSION_STRING "v1.0.6"
#define	HPSJAM_ICON_FILE ":/HpsJam.png"
#define	HPSJAM_PEERS_MAX 256
#define	HPSJAM_SEQ_MAX 16
#define	HPSJAM_NUM_ICONS 14
#define	HPSJAM_AUDIO_FORMAT_MAX 9
#define	HPSJAM_ICON_SIZE 64 /* 64x64 px SVG */
#define	HPSJAM_MAX_UDP 2048 /* bytes (need to have room for two packets) */
#define	HPSJAM_DEFAULT_IPV4_PORT 22124
#define	HPSJAM_DEFAULT_IPV6_PORT 22124
#define	HPSJAM_DEFAULT_IPV4_PORT_STR "22124"
#define	HPSJAM_DEFAULT_IPV6_PORT_STR "22124"
#define	HPSJAM_BIT_MUTE (1 << 0)
#define	HPSJAM_BIT_SOLO (1 << 1)
#define	HPSJAM_BIT_INVERT (1 << 2)
#define	HPSJAM_BIT_GAIN_SET(x) (((x) & 31) << 3)
#define	HPSJAM_BIT_GAIN_GET(x) (((x) >> 3) & 31)

#define	HPSJAM_NO_SIGNAL(a,b) do {	\
  a.blockSignals(true);			\
  a.b;					\
  a.blockSignals(false);		\
} while (0)
#define	HPSJAM_SWAP(a,b) do {		\
	typeof(a) __temp = (b);		\
	(b) = (a);			\
	(a) = __temp;			\
} while (0)

class hpsjam_server_peer;
class hpsjam_client_peer;
class HpsJamClient;
class QMutex;
struct hpsjam_socket_address;

extern uint64_t hpsjam_server_passwd;
extern uint64_t hpsjam_mixer_passwd;
extern unsigned hpsjam_num_server_peers;
extern unsigned hpsjam_udp_buffer_size;
extern class hpsjam_server_peer *hpsjam_server_peers;
extern class hpsjam_client_peer *hpsjam_client_peer;
extern class HpsJamClient *hpsjam_client;
extern struct hpsjam_socket_address hpsjam_v4;
extern struct hpsjam_socket_address hpsjam_v6;
extern struct hpsjam_socket_address hpsjam_cli;
extern const char *hpsjam_welcome_message_file;

extern void hpsjam_socket_init(unsigned short ipv4_port,
    unsigned short ipv6_port, unsigned short cliport);

/* sound APIs */
extern bool hpsjam_sound_init(const char *, bool);
extern void hpsjam_sound_uninit();
extern int hpsjam_sound_toggle_input_device(int);
extern int hpsjam_sound_toggle_output_device(int);
extern int hpsjam_sound_toggle_input_channel(int, int);
extern int hpsjam_sound_toggle_output_channel(int, int);
extern int hpsjam_sound_max_input_channel();
extern int hpsjam_sound_max_output_channel();

class QString;
extern void hpsjam_sound_get_input_status(QString &);
extern void hpsjam_sound_get_output_status(QString &);

#endif		/* _HPSJAM_H_ */
