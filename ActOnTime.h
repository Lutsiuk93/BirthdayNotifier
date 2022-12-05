#pragma once
#include <qtimer.h>

class BirthdayNotifierModel;

class ActOnTimer : public QTimer
{
public:
	ActOnTimer(BirthdayNotifierModel* ip_model);
	void SetCheckedAt(QDate i_checked_date);
	QDate* CheckedAt() const;

private:
	BirthdayNotifierModel* mp_model = nullptr;
	std::unique_ptr<QDate> checked_at = nullptr;

private:
	void timerEvent(QTimerEvent* evt) override;
};