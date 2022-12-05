#include "BirthdayNotifierModel.h"
#include <memory>
#include "qcolor.h"
#include "qfile.h"

void SetHeader(Header i_header, QAbstractItemModel* i_model)
{
	QString title("");
	if (i_header == Header::FirstName)
		title = "First Name";
	else if (i_header == Header::LastName)
		title = "Last Name";
	else if (i_header == Header::BirthdayDate)
		title = "Birthday Date";
	else if (i_header == Header::NotifyFor)
		title = "Notify for";

	int column = static_cast<int>(i_header);
	i_model->insertColumn(column);
	i_model->setHeaderData(column, Qt::Horizontal, title);

}

BirthdayNotifierModel::BirthdayNotifierModel() : m_act_on_timer(this)
{
	SetHeader(Header::FirstName, this);
	SetHeader(Header::LastName, this);
	SetHeader(Header::BirthdayDate, this);
	SetHeader(Header::NotifyFor, this);
}

BirthdayNotifierModel::~BirthdayNotifierModel()
{
	SaveToFile();
}

void BirthdayNotifierModel::SetIndex(const QModelIndex& index)
{
	m_selected_index = index;
	auto isIndexValid = m_selected_index.isValid();
	if (isIndexValid) {
		const auto& selected_ntf = m_birthdays.at((m_birthdays.size() - 1) - m_selected_index.row());
		SetFirstName(selected_ntf.GetFirstName(), true);
		SetLastName(selected_ntf.GetLastName(), true);
		SetBirthdayDate(selected_ntf.GetBirthdayDate(), true);
		SetDaysToNotify(selected_ntf.GetDaysToNotify(), true);
	}
	emit IndexChanged(m_selected_index.isValid());
}

void BirthdayNotifierModel::_ColorRow(QColor i_color, int i_row_index)
{
	QModelIndex index_to_color = index(i_row_index, static_cast<int>(Header::FirstName));
	setData(index_to_color, i_color, Qt::BackgroundRole);
	index_to_color = index(i_row_index, static_cast<int>(Header::LastName));
	setData(index_to_color, i_color, Qt::BackgroundRole);
	index_to_color = index(i_row_index, static_cast<int>(Header::BirthdayDate));
	setData(index_to_color, i_color, Qt::BackgroundRole);
	index_to_color = index(i_row_index, static_cast<int>(Header::NotifyFor));
	setData(index_to_color, i_color, Qt::BackgroundRole);
}

void BirthdayNotifierModel::_FillRow(const int i_row_index, const BirthdayNotification& i_bday_ntf)
{
	QModelIndex insertion_index = index(i_row_index, static_cast<int>(Header::FirstName));
	setData(insertion_index, i_bday_ntf.GetFirstName());
	insertion_index = index(i_row_index, static_cast<int>(Header::LastName));
	setData(insertion_index, i_bday_ntf.GetLastName());
	insertion_index = index(i_row_index, static_cast<int>(Header::BirthdayDate));
	setData(insertion_index, i_bday_ntf.GetBirthdayDate().toString("yyyy.MM.dd"));
	insertion_index = index(i_row_index, static_cast<int>(Header::NotifyFor));
	setData(insertion_index, QVariant::fromValue(i_bday_ntf.GetDaysToNotify()));
}

void BirthdayNotifierModel::AddNotification()
{
	BirthdayNotification ntf_to_add = m_edt_notification;
	m_birthdays.push_back(ntf_to_add);
	insertRow(0);
	_FillRow(0, ntf_to_add);
}

void BirthdayNotifierModel::EditNotification()
{
	m_birthdays.at((m_birthdays.size() - 1) - m_selected_index.row()) = m_edt_notification;
	_FillRow(m_selected_index.row(), m_edt_notification);
	_ColorRow(QColor(255, 255, 255), m_selected_index.row());
}

void BirthdayNotifierModel::RemoveNotification()
{
	int storage_index = (m_birthdays.size() - 1) - m_selected_index.row();
	m_birthdays.erase(m_birthdays.begin() + storage_index);
	removeRow(m_selected_index.row());
}

void BirthdayNotifierModel::CheckOut()
{
	QStringList upcoming_bdays;
	auto current_date = QDate::currentDate();

	for (int index = 0; index < m_birthdays.size(); index++)
	{
		const auto& bday_ntf = m_birthdays.at(index);
		auto bday_date = bday_ntf.GetBirthdayDate();

		// We add "days to notify" to current date, to check with birthday date
		auto new_date = current_date.addDays(bday_ntf.GetDaysToNotify());
		if (new_date.month() == bday_date.month())
			if (int days_difference = new_date.day() - bday_date.day();
				days_difference >= 0 && days_difference <= bday_ntf.GetDaysToNotify()) {

			bool isToday = days_difference == bday_ntf.GetDaysToNotify();
			QColor row_color = isToday ? QColor(144, 238, 144) : QColor(255, 255, 204);
			_ColorRow(row_color, (m_birthdays.size() - 1) - index);
			upcoming_bdays.push_back(QString("%1 %2 \n").arg(bday_ntf.GetFirstName() + " " + bday_ntf.GetLastName())
				.arg(isToday ? "have a birthday today" :
					QString("will have a birthday in %1 day(s)").arg(bday_ntf.GetDaysToNotify() - days_difference)));
		}

	}

	if (!upcoming_bdays.isEmpty())
		emit BirthdaysFound(upcoming_bdays);

};


void BirthdayNotifierModel::SetFirstName(QString i_first_name, bool i_force) {
	if (i_first_name == m_edt_notification.GetFirstName() && !i_force)
		return;
	m_edt_notification.SetFirstName(i_first_name);
	emit FirstNameChanged(i_first_name);
};
void BirthdayNotifierModel::SetLastName(QString i_last_name, bool i_force) {
	if (i_last_name == m_edt_notification.GetLastName() && !i_force)
		return;
	m_edt_notification.SetLastName(i_last_name);
	emit LastNameChanged(i_last_name);
};
void BirthdayNotifierModel::SetDaysToNotify(std::size_t i_days_to_notify, bool i_force) {
	if (i_days_to_notify == m_edt_notification.GetDaysToNotify() && !i_force)
		return;
	m_edt_notification.SetDaysToNotify(i_days_to_notify);
	emit DaysToNotifyChanged(i_days_to_notify);
};
void BirthdayNotifierModel::SetBirthdayDate(QDate i_bday_date, bool i_force) {
	if (i_bday_date == m_edt_notification.GetBirthdayDate() && !i_force)
		return;
	m_edt_notification.SetBirthdayDate(i_bday_date);
	emit BirthdayDateChanged(i_bday_date);
};

void BirthdayNotifierModel::SetNotificationTime(QTime i_time)
{
	m_notification_time = i_time;
	emit NotificationTimeChanged(m_notification_time);
}

QTime BirthdayNotifierModel::GetTime() const
{
	return m_notification_time;
}

void BirthdayNotifierModel::SaveToFile() const
{
	QFile ntf_storage("notifications_storage");
	if (ntf_storage.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		QDataStream save(&ntf_storage);
		save.setVersion(QDataStream::Qt_6_1);

		for (auto ntf : m_birthdays) {
			save << ntf;
		}

		ntf_storage.close();
	}
	QFile time_storage("time_storage");
	if (time_storage.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		QDataStream save(&time_storage);
		save.setVersion(QDataStream::Qt_6_1);
		save << m_notification_time;
		QDate checkedat;
		if (!m_act_on_timer.CheckedAt())
			save << NULL;
		else
			save << *m_act_on_timer.CheckedAt();
		time_storage.close();
	}
}

void BirthdayNotifierModel::LoadFromFile()
{
	QFile ntf_storage("notifications_storage");
	if (ntf_storage.open(QIODevice::ReadOnly))
	{
		QDataStream load(&ntf_storage);
		load.setVersion(QDataStream::Qt_6_1);

		while (!load.atEnd())
		{
			load >> m_edt_notification;
			AddNotification();
		}
		ntf_storage.close();
	}
	QFile time_storage("time_storage");
	if (time_storage.open(QIODevice::ReadOnly))
	{
		QDataStream load(&time_storage);
		load.setVersion(QDataStream::Qt_6_1);
		load >> m_notification_time;
		QDate checkedat;
		load >> checkedat;
		SetNotificationTime(m_notification_time);
		m_act_on_timer.SetCheckedAt(checkedat);
		time_storage.close();
	}
}
