#pragma once
#include <QObject>
#include <vector>
#include "BirthdayNotification.h"
#include <memory>
#include "qstandarditemmodel.h"
#include "ActOnTime.h"

enum class Header
{
	FirstName = 0,
	LastName,
	BirthdayDate,
	NotifyFor
};

class BirthdayNotifierModel : public QStandardItemModel
{
	Q_OBJECT

public:
	BirthdayNotifierModel();
	~BirthdayNotifierModel();

public:
	QTime GetTime() const;

public slots:
	void SetIndex(const QModelIndex& index);
	void SetFirstName(QString i_first_name, bool i_force = false);
	void SetLastName(QString i_last_name, bool i_force = false);
	void SetDaysToNotify(std::size_t i_days_to_notify, bool i_force = false);
	void SetBirthdayDate(QDate i_date, bool i_force = false);
	void SetNotificationTime(QTime i_time);
	void AddNotification();
	void EditNotification();
	void RemoveNotification();
	void CheckOut();
	void SaveToFile() const;
	void LoadFromFile();

signals:
	void BirthdaysFound(QStringList o_birthdays_list);
	void NotificationAdded(QString o_ntf);
	void IndexChanged(bool o_is_enable);
	void FirstNameChanged(QString o_new_first_name);
	void LastNameChanged(QString o_new_last_name);
	void BirthdayDateChanged(QDate o_new_date);
	void DaysToNotifyChanged(long o_new_days);
	void NotificationTimeChanged(QTime o_new_time);


private:
	void _ColorRow(QColor i_color, int i_row_index);
	void _FillRow(const int i_row_index, const BirthdayNotification& i_bday_ntf);
private:
	BirthdayNotification m_edt_notification;
	std::vector<BirthdayNotification> m_birthdays;
	QModelIndex m_selected_index;
	QTime m_notification_time = QTime::currentTime();
	ActOnTimer m_act_on_timer;
};