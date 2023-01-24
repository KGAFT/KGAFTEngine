//
// Created by Danil on 13.09.2022.
//


#pragma once

class KeyBoardCallBack {
public:
    virtual unsigned int getKeyCodeAmount() = 0;

    virtual unsigned int *getKeyCodes() = 0;

    virtual void action(unsigned int keyCode) = 0;

};

