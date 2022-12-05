#include "BirthdayNotifierWindow.h"
#include "qstandarditemmodel.h"
#include "qmessagebox.h"
#include "qmenu.h"

BirthdayNotifierWindow::BirthdayNotifierWindow(BirthdayNotifierModel* i_model, QWidget* parent)
	: QMainWindow(parent), mp_model(i_model)
{
	m_ui.setupUi(this);
	///////////////////////////////////////////////////////////////////////////////////////////
	// adjust table view
	m_ui.tableView->setModel(i_model);
	m_ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	///////////////////////////////////////////////////////////////////////////////////////////
	// adjust tray
	m_tray_icon.setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));
	m_tray_icon.setToolTip("Birthday notifier\n");
	QMenu* menu = new QMenu(this);
	QAction* viewWindow = new QAction(QString("Open window").toUtf8(), this);
	QAction* quitAction = new QAction(QString("Close application"), this);
	auto is_connected = QObject::connect(viewWindow, &QAction::triggered, this, &BirthdayNotifierWindow::show);
	Q_ASSERT(is_connected);
	is_connected = QObject::connect(quitAction, &QAction::triggered, this, &BirthdayNotifierWindow::close);
	Q_ASSERT(is_connected);
	menu->addAction(viewWindow);
	menu->addAction(quitAction);
	m_tray_icon.setContextMenu(menu);
	m_tray_icon.show();
	///////////////////////////////////////////////////////////////////////////////////////////

	m_ui.notify_for_edit->setValidator(new QIntValidator(0, 100, this));
	_SetUpConnections();
	m_ui.bithday_date_edit->setDate(QDate::currentDate());
	_UpdateControls();
	m_ui.remove_btn->setEnabled(false);
	m_ui.edit_btn->setEnabled(false);
}

BirthdayNotifierWindow::~BirthdayNotifierWindow()
{}

void BirthdayNotifierWindow::_ShowUpcomingBirthdays(QStringList& i_upcoming_bdays)
{
	QString msg_text("");
	for (const auto& str : i_upcoming_bdays)
		msg_text += str;
	if (this->isVisible()) {
		QMessageBox msg;
		msg.setText(msg_text);
		msg.exec();
	}
	else
	{
		QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
		m_tray_icon.showMessage(QString("Birthday Notifier").toUtf8(),
			QString(msg_text.toUtf8()).toUtf8(),
			icon,
			2000);
	}
}

void BirthdayNotifierWindow::_UpdateControls()
{
	bool enable_add_btn = !m_ui.first_name_edit->text().isEmpty() && !m_ui.last_name_edit->text().isEmpty() &&
		!m_ui.notify_for_edit->text().isEmpty();

	m_ui.add_btn->setEnabled(enable_add_btn);
}

void BirthdayNotifierWindow::_SetUpConnections()
{
	auto isConnected = QObject::connect(m_ui.add_btn, &QAbstractButton::clicked, mp_model, &BirthdayNotifierModel::AddNotification);
	Q_ASSERT(isConnected);

	isConnected = QObject::connect(m_ui.first_name_edit, &QLineEdit::textChanged, this, [&](const QString i_new_first_name) {
		mp_model->SetFirstName(i_new_first_name);
		_UpdateControls(); });
	Q_ASSERT(isConnected);

	isConnected = QObject::connect(mp_model, &BirthdayNotifierModel::FirstNameChanged, this, [&](QString i_new_first_name) {
		m_ui.first_name_edit->setText(i_new_first_name);
		});
	Q_ASSERT(isConnected);

	isConnected = QObject::connect(m_ui.last_name_edit, &QLineEdit::textChanged, this, [&](const QString i_new_last_name) {
		mp_model->SetLastName(i_new_last_name);
		_UpdateControls(); });
	Q_ASSERT(isConnected);

	isConnected = QObject::connect(mp_model, &BirthdayNotifierModel::LastNameChanged, this, [&](QString i_new_last_name) {
		m_ui.last_name_edit->setText(i_new_last_name);
		});
	Q_ASSERT(isConnected);

	isConnected = QObject::connect(m_ui.bithday_date_edit, &QDateEdit::dateChanged, this, [&](QDate i_new_date) {
		mp_model->SetBirthdayDate(i_new_date);
		_UpdateControls(); });
	Q_ASSERT(isConnected);

	isConnected = QObject::connect(mp_model, &BirthdayNotifierModel::BirthdayDateChanged, this, [&](QDate i_new_bday_date) {
		m_ui.bithday_date_edit->setDate(i_new_bday_date);
		});
	Q_ASSERT(isConnected);

	isConnected = QObject::connect(m_ui.notify_for_edit, &QLineEdit::textChanged, this, [&](const QString i_new_days_to_notify) {
		mp_model->SetDaysToNotify(i_new_days_to_notify.toLong());
		_UpdateControls(); });
	Q_ASSERT(isConnected);

	isConnected = QObject::connect(mp_model, &BirthdayNotifierModel::DaysToNotifyChanged, this, [&](long i_new_days_to_notify) {
		m_ui.notify_for_edit->setText(QString::number(i_new_days_to_notify));
		});
	Q_ASSERT(isConnected);

	isConnected = QObject::connect(m_ui.tableView, &QTableView::clicked, mp_model, &BirthdayNotifierModel::SetIndex);
	Q_ASSERT(isConnected);

	isConnected = QObject::connect(m_ui.remove_btn, &QAbstractButton::clicked, mp_model, [&]() {
		mp_model->RemoveNotification();
		mp_model->SetIndex(m_ui.tableView->selectionModel()->currentIndex());
		});
	Q_ASSERT(isConnected);

	isConnected = QObject::connect(mp_model, &BirthdayNotifierModel::IndexChanged, this, [&](bool is_valid_index)
		{
			m_ui.remove_btn->setEnabled(is_valid_index);
			m_ui.edit_btn->setEnabled(is_valid_index);
		});
	Q_ASSERT(isConnected);

	isConnected = QObject::connect(m_ui.check_out_btn, &QAbstractButton::clicked, mp_model, &BirthdayNotifierModel::CheckOut);
	Q_ASSERT(isConnected);

	isConnected = QObject::connect(mp_model, &BirthdayNotifierModel::BirthdaysFound, this, [&](QStringList i_bdays_list)
		{
			_ShowUpcomingBirthdays(i_bdays_list);
		});
	Q_ASSERT(isConnected);

	isConnected = QObject::connect(m_ui.edit_btn, &QAbstractButton::clicked, mp_model, &BirthdayNotifierModel::EditNotification);
	Q_ASSERT(isConnected);

	isConnected = QObject::connect(m_ui.set_time_btn, &QAbstractButton::clicked, mp_model, [&]()
		{
			mp_model->SetNotificationTime(m_ui.notification_time->time());
		});
	Q_ASSERT(isConnected);

	isConnected = QObject::connect(mp_model, &BirthdayNotifierModel::NotificationTimeChanged, this, [&](QTime i_new_time)
		{
			m_ui.notification_time->setTime(i_new_time);
		});

	Q_ASSERT(isConnected);

	isConnected = QObject::connect(&m_tray_icon, &QSystemTrayIcon::activated,
		this, &BirthdayNotifierWindow::IconActivated);
	Q_ASSERT(isConnected);
}

void BirthdayNotifierWindow::closeEvent(QCloseEvent* event)
{
	if (this->isVisible()) {
		event->ignore();
		this->hide();
		QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
		m_tray_icon.showMessage(QString("Birthday Notifier").toUtf8(),
			QString("The application was hidden to tray").toUtf8(),
			icon,
			2000);
	}
}

void BirthdayNotifierWindow::IconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
	case QSystemTrayIcon::Trigger:
		if (!this->isVisible())
			this->show();
		else
			this->hide();
		break;
	default:
		break;
	}
}