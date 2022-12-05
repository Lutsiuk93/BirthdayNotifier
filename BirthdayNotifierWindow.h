#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BirthdayNotifierWindow.h"
#include "BirthdayNotifierModel.h"
#include <QCloseEvent>
#include <qsystemtrayicon.h>

class BirthdayNotifierWindow : public QMainWindow
{
	Q_OBJECT

public:
	BirthdayNotifierWindow(BirthdayNotifierModel* model, QWidget* parent = nullptr);
	~BirthdayNotifierWindow();

protected:
	void closeEvent(QCloseEvent* event) override;

private:
	void _SetUpConnections();
	void _UpdateControls();
	void _ShowUpcomingBirthdays(QStringList& i_upcoming_bdays);

private slots:
	void IconActivated(QSystemTrayIcon::ActivationReason reason);

private:
	Ui::BirthdayNotifierWindowClass m_ui;
	BirthdayNotifierModel* mp_model;
	QSystemTrayIcon m_tray_icon;
};
