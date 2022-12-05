#include "BirthdayNotification.h"
#include <QString>

BirthdayNotification::BirthdayNotification(QString i_first_name, QString i_last_name,
	long i_days_to_notify, QDate i_birthday_date)
	: first_name(i_first_name), last_name(i_last_name),
	days_to_notify(i_days_to_notify), birthday_date(i_birthday_date) {};

void BirthdayNotification::SetFirstName(QString i_first_name) { first_name = i_first_name; }
QString BirthdayNotification::GetFirstName() const { return first_name; }
void BirthdayNotification::SetLastName(QString i_last_name) { last_name = i_last_name; }
QString BirthdayNotification::GetLastName() const { return last_name; }
void BirthdayNotification::SetDaysToNotify(long i_days_to_notify) { days_to_notify = i_days_to_notify; }
long BirthdayNotification::GetDaysToNotify() const { return days_to_notify; }
void BirthdayNotification::SetBirthdayDate(QDate i_bday_date) { birthday_date = i_bday_date; }
QDate BirthdayNotification::GetBirthdayDate() const { return birthday_date; }

QString BirthdayNotification::toQString() const
{
	return QString("%1 %2 %3 Days To Notify: %4").arg(
		first_name).arg(last_name).arg(birthday_date.toString("yyyy.MM.dd")).arg(days_to_notify);

}