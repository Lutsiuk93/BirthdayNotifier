#pragma once
#include <QDate>
#include <QString>
#include <qdatastream.h>

class BirthdayNotification
{
public:
	BirthdayNotification(QString i_first_name, QString i_last_name, long i_days_to_notify, QDate i_birthday_date);
	BirthdayNotification() = default;
	void SetFirstName(QString i_first_name);
	QString GetFirstName() const;
	void SetLastName(QString i_last_name);
	QString GetLastName() const;
	void SetDaysToNotify(long i_days_to_notify);
	long GetDaysToNotify() const;
	void SetBirthdayDate(QDate i_bday_date);
	QDate GetBirthdayDate() const;
	QString toQString() const;

	/* overload the operators */
	friend QDataStream& operator<< (QDataStream& out, const BirthdayNotification& rhs)
	{
		out << rhs.first_name << rhs.last_name << rhs.days_to_notify << rhs.birthday_date;
		return out;
	}

	friend QDataStream& operator>> (QDataStream& in, BirthdayNotification& rhs)
	{
		in >> rhs.first_name >> rhs.last_name >> rhs.days_to_notify >> rhs.birthday_date;
		return in;
	}

private:
	QString first_name;
	QString last_name;
	std::size_t days_to_notify;
	QDate birthday_date;
};