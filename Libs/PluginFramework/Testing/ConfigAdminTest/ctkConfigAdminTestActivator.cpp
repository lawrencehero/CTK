/*=============================================================================

  Library: CTK

  Copyright (c) German Cancer Research Center,
    Division of Medical and Biological Informatics

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=============================================================================*/

#include "ctkConfigAdminTestActivator_p.h"

#include <ctkPluginContext.h>
#include <ctkPluginConstants.h>

#include <QtPlugin>
#include <QStringList>

#include "ctkConfigurationPluginTestSuite_p.h"
#include "ctkConfigurationListenerTestSuite_p.h"

void ctkConfigAdminTestActivator::start(ctkPluginContext* context)
{
  QString symbolicName = context->getProperty("cm.impl").toString();
  if (symbolicName.isEmpty())
  {
    throw std::logic_error("Framework property 'cm.impl' containing the symbolic "
                           "name of the ConfigAdmin implementation not found!");
  }

  long cmPluginId = -1;
  foreach(QSharedPointer<ctkPlugin> p, context->getPlugins())
  {
    if (p->getSymbolicName() == symbolicName)
    {
      cmPluginId = p->getPluginId();
      break;
    }
  }

  if (cmPluginId < 0)
  {
    QString msg = QString("The ConfigAdmin implementation '%1' is not installed.")
        .arg(symbolicName);
    throw std::logic_error(msg.toStdString());
  }

  ServiceProperties props;

  configPluginTestSuite = new ctkConfigurationPluginTestSuite(context, cmPluginId);
  props.clear();
  props.insert(ctkPluginConstants::SERVICE_PID, configPluginTestSuite->metaObject()->className());
  context->registerService<ctkTestSuiteInterface>(configPluginTestSuite, props);

  configListenerTestSuite = new ctkConfigurationListenerTestSuite(context, cmPluginId);
  props.clear();
  props.insert(ctkPluginConstants::SERVICE_PID, configListenerTestSuite->metaObject()->className());
  context->registerService<ctkTestSuiteInterface>(configListenerTestSuite, props);
}

void ctkConfigAdminTestActivator::stop(ctkPluginContext* context)
{
  Q_UNUSED(context);

  //delete frameworkTestSuite;
  //delete serviceListenerTestSuite;
}

Q_EXPORT_PLUGIN2(org_commontk_configadmintest, ctkConfigAdminTestActivator)