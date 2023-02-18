#include "uuidgeneratormodulewidget.hpp"
#include "ui_uuidgeneratormodulewidget.h"

#include <QUuid>

UUIDGeneratorModuleWidget::UUIDGeneratorModuleWidget(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::UUIDGeneratorModuleWidget)
{
    ui->setupUi(this);
}

UUIDGeneratorModuleWidget::~UUIDGeneratorModuleWidget()
{
    delete ui;
}
