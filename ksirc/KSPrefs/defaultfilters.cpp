/**********************************************************************

	--- Qt Architect generated file ---

	File: defaultfilters.cpp
	Last generated: Mon Feb 16 20:17:25 1998

 *********************************************************************/

#include "defaultfilters.h"
#include "../config.h"

#include <kconfig.h>

extern KConfig *kConfig;
extern global_config *kSircConfig;

#define Inherited defaultfiltersdata

DefaultFilters::DefaultFilters
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
  kSircColours->setChecked(!kSircConfig->filterKColour);
  mircColours->setChecked(!kSircConfig->filterMColour);
  NickFColour->setRange(-1,15);
  NickFColour->setValue(kSircConfig->nickFHighlight);
  connect(NickFColour, SIGNAL(valueIncreased()),
	  this, SLOT(slot_fgset()));
  connect(NickFColour, SIGNAL(valueDecreased()),
	  this, SLOT(slot_fgset()));
  NickBColour->setRange(-1,15);
  NickBColour->setValue(kSircConfig->nickBHighlight);
  if(kSircConfig->nickFHighlight == -1){
    NickBColour->hide();
    LabelNickB->hide();
  }
  usHighlightColour->setRange(-1,15);
  usHighlightColour->setValue(kSircConfig->usHighlight);


}


DefaultFilters::~DefaultFilters()
{
}

void DefaultFilters::slot_apply()
{
  kSircConfig->filterKColour = !kSircColours->isChecked();
  kSircConfig->filterMColour = !mircColours->isChecked();
  kSircConfig->nickFHighlight = NickFColour->getValue();
  kSircConfig->nickBHighlight = NickBColour->getValue();
  kSircConfig->usHighlight = usHighlightColour->getValue();
  kConfig->setGroup("Colours");
  kConfig->writeEntry("kcolour", kSircConfig->filterKColour);
  kConfig->writeEntry("mcolour", kSircConfig->filterMColour);
  kConfig->writeEntry("nickfcolour", kSircConfig->nickFHighlight);
  kConfig->writeEntry("nickbcolour", kSircConfig->nickBHighlight); 
  kConfig->writeEntry("uscolour", kSircConfig->usHighlight);
}

void DefaultFilters::slot_fgset()
{
  if(NickFColour->getValue() == -1){
    NickBColour->hide();
    LabelNickB->hide();
  }
  else{
    NickBColour->show();
    LabelNickB->show();
  }


}
