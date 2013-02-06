#include <QLabel>
#include <QGridLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>
#include <QDebug>

#include "Accounts.hpp"
#include "RegalisNewsletter.hpp"

Accounts::Accounts() {
}

Accounts::AccountForm *Accounts::getNewAccountForm() {
	return new AccountForm();
}

Accounts::AccountForm::AccountForm() {
	QLabel *name_label = new QLabel(tr("Name:"));
	name = new QLineEdit();
	name_label->setBuddy(name);

	QLabel *email_label = new QLabel(tr("Email:"));
	email = new QLineEdit();
	email_label->setBuddy(email);

	QLabel *from_label = new QLabel(tr("From:"));
	from = new QLineEdit();
	from_label->setBuddy(from);

	QLabel *host_label = new QLabel(tr("SMTP Host:"));
	host = new QLineEdit();
	host_label->setBuddy(host);

	QLabel *user_label = new QLabel(tr("User:"));
	user = new QLineEdit();
	user_label->setBuddy(user);

	QLabel *pass_label = new QLabel(tr("Password:"));
	pass = new QLineEdit();
	pass->setEchoMode(QLineEdit::Password);
	pass_label->setBuddy(pass);

	QLabel *port_label = new QLabel(tr("SMTP Port:"));
	port = new QLineEdit("25");
	port->setInputMask("999999999");
	port_label->setBuddy(port);

	QLabel *description_label = new QLabel(tr("Description:"));
	description = new QTextEdit(tr("Default account"));
	description_label->setBuddy(description);

	QGridLayout *layout = new QGridLayout();

	layout->addWidget(name_label, 0, 0, Qt::AlignRight);
	layout->addWidget(name, 0, 1);

	layout->addWidget(email_label, 1, 0, Qt::AlignRight);
	layout->addWidget(email, 1, 1);

	layout->addWidget(from_label, 2, 0, Qt::AlignRight);
	layout->addWidget(from, 2, 1);

	layout->addWidget(host_label, 3, 0, Qt::AlignRight);
	layout->addWidget(host, 3, 1);

	layout->addWidget(port_label, 4, 0, Qt::AlignRight);
	layout->addWidget(port, 4, 1);

	layout->addWidget(user_label, 5, 0, Qt::AlignRight);
	layout->addWidget(user, 5, 1);

	layout->addWidget(pass_label, 6, 0, Qt::AlignRight);
	layout->addWidget(pass, 6, 1);

	layout->addWidget(description_label, 7, 0, Qt::AlignRight | Qt::AlignTop);
	layout->addWidget(description, 7, 1);

	layout->setColumnMinimumWidth(1, 250);
	setLayout(layout);

}

bool Accounts::AccountForm::initModel() {
	model = new QSqlTableModel();
	model->setTable("accounts");
	model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	model->setSort(1, Qt::AscendingOrder);
	return model->select();
}

bool Accounts::AccountForm::validate() {
	if (name->text().isEmpty() || email->text().isEmpty() ||
		from->text().isEmpty() || host->text().isEmpty() ||
		user->text().isEmpty() || pass->text().isEmpty() ||
		port->text().isEmpty()) {
		error_msg = tr("Some fields are empty");
		return false;
	}
	// TODO: Check server connection
	return true;
}

bool Accounts::AccountForm::insert() {
	QSqlRecord record = model->record();
	record.remove(0);
	record.setValue("name", getName());
	record.setValue("email", getEmail());
	record.setValue("header_from", getFrom());
	record.setValue("smtp_host", getHost());
	record.setValue("smtp_user", getUser());
	record.setValue("smtp_pass", getPass());
	record.setValue("smtp_port", getPort().toInt());
	record.setValue("description", getDescription());
	if (!(model->insertRecord(-1, record) && model->submitAll())) {
		error_msg = model->lastError().text();
		return false;
	}
	return true;
}

QString Accounts::AccountForm::getName() const {
	return name->text();
}

QString Accounts::AccountForm::getEmail() const {
	return email->text();
}

QString Accounts::AccountForm::getFrom() const {
	return from->text();
}

QString Accounts::AccountForm::getHost() const {
	return host->text();
}

QString Accounts::AccountForm::getUser() const {
	return user->text();
}

QString Accounts::AccountForm::getPass() const {
	return pass->text();
}

QString Accounts::AccountForm::getPort() const {
	return port->text();
}

QString Accounts::AccountForm::getDescription() const {
	return description->toPlainText();
}

QString Accounts::AccountForm::getErrorMessage() const {
	return error_msg;
}
