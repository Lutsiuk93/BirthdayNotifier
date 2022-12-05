#include "ActOnTime.h"
#include "BirthdayNotifierModel.h"
#include <qtimer.h>


void ActOnTimer::SetCheckedAt(QDate i_checked_date)
{
	checked_at = std::make_unique<QDate>(i_checked_date);
}


ActOnTimer::ActOnTimer(BirthdayNotifierModel* ip_model) : mp_model(ip_model)
{
	auto isConnected = QObject::connect(mp_model, &BirthdayNotifierModel::NotificationTimeChanged, this, [&]()
		{
			checked_at.reset(nullptr);
		});

	start(1000);
}

QDate* ActOnTimer::CheckedAt() const
{
	return checked_at.get();
}

void ActOnTimer::timerEvent(QTimerEvent* evt)
{
	if (!checked_at || *checked_at.get() < QDate::currentDate()) {
		if (QTime::currentTime() >= mp_model->GetTime()) {
			mp_model->CheckOut();
			SetCheckedAt(QDate::currentDate());
		}
	}
}
