#include "settingsdialog.h"
#include <QSettings>
#include <QFontDatabase>

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent)
{
	setWindowTitle(tr("Settings"));

	QLabel* fontLabel = new QLabel(tr("Font:"));
	fontComboBox = new QFontComboBox;

	QLabel* sizeLabel = new QLabel(tr("Size:"));
	fontSizeSpinBox = new QSpinBox;
	fontSizeSpinBox->setRange(6, 72);
	fontSizeSpinBox->setValue(10); // Default

	// Load current settings
	QSettings settings;
	QString fontFamily = settings.value("editor/fontFamily", "Courier New").toString();
	int fontSize = settings.value("editor/fontSize", 10).toInt();

	QFont currentFont(fontFamily);
	fontComboBox->setCurrentFont(currentFont);
	fontSizeSpinBox->setValue(fontSize);

	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::saveSettings);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(fontLabel);
	mainLayout->addWidget(fontComboBox);
	mainLayout->addWidget(sizeLabel);
	mainLayout->addWidget(fontSizeSpinBox);
	mainLayout->addWidget(buttonBox);

	setLayout(mainLayout);
}

void SettingsDialog::saveSettings()
{
	QSettings settings;
	settings.setValue("editor/fontFamily", fontComboBox->currentFont().family());
	settings.setValue("editor/fontSize", fontSizeSpinBox->value());
	accept();
}

QFont SettingsDialog::currentFont() const
{
	return fontComboBox->currentFont();
}

int SettingsDialog::currentFontSize() const
{
	return fontSizeSpinBox->value();
}
