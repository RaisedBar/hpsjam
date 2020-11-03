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

#ifndef _HPSJAM_MIXERDLG_H_
#define	_HPSJAM_MIXERDLG_H_

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QRect>
#include <QPoint>
#include <QSvgRenderer>
#include <QLabel>

class HpsJamIcon : public QWidget {
public:
	HpsJamIcon();

	QSvgRenderer svg;

	void paintEvent(QPaintEvent *);
};

class HpsJamSlider : public QWidget {
	Q_OBJECT;
public:
	HpsJamSlider();

	bool active;
	QRect target;
	QPoint start;
	float value;
	float level;

	void setValue(float);
	void setLevel(float);

	void paintEvent(QPaintEvent *);
        void mousePressEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *);
        void mouseReleaseEvent(QMouseEvent *);
signals:
	void valueChanged();
};

class HpsJamStrip : public QWidget {
	Q_OBJECT;
public:
	HpsJamStrip(int);

	int id;

	QGridLayout *gl;
	QLabel *w_name;
	HpsJamIcon *w_icon;
	HpsJamSlider *w_slider;
	QPushButton *w_eq;
	QPushButton *w_inv;
	QPushButton *w_mute;
	QPushButton *w_solo;

public slots:
	void handleSlider();
	void handleSolo();
	void handleMute();
	void handleEQ();
	void handleInv();

signals:
	void valueChanged(int);
};

class HpsJamMixer : public QWidget {

};

#endif		/* _HPSJAM_MIXERDLG_H_ */
