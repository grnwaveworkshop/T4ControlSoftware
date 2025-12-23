#ifndef EVENTS_HPP
#define EVENTS_HPP

void ProcessEventsLoop();
void CheckButtonPress(void (*funcA)(), void (*funcB)());
void CheckLongPress(void (*func)(), bool continuous);
void ProcessButtons();
void ButtonA1Click();
void ButtonA1LongClick();
void ButtonA2Click();
void ButtonA2ClickB();
void ButtonA2LongClick();
void ButtonA3Click();
void ButtonA3ClickB();
void ButtonA4Click();
void ButtonA4ClickB();
void ButtonB1Click();
void ButtonB1ClickB();
void ButtonB2Click();
void ButtonB2ClickB();
void ButtonB2LongClick();
void ButtonB3Click();
void ButtonB3ClickB();
void ButtonB4Click();
void ButtonB4ClickB();
void ButtonB4LongClick();
void ButtonC1Click();
void ButtonC1ClickB();
void ButtonC1LongClick();
void ButtonC2Click();
void ButtonC2ClickB();
void ButtonC3Click();
void ButtonC3ClickB();
void ButtonC4Click();
void ButtonC4LongClick();
void ButtonC4ClickB();
void ButtonD1Click();
void ButtonD1ClickB();
void ButtonD1LongClick();
void ButtonD2Click();
void ButtonD2ClickB();
void ButtonD3Click();
void ButtonD3ClickB();
void ButtonD3LongClick();
void ButtonD4Click();
void ButtonD4ClickB();
void ButtonD4LongClick();
bool isDataPanelOpen();

#endif