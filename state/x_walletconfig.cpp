// Copyright 2019 The MWC Developers
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "x_walletconfig.h"
#include "../wallet/wallet.h"
#include "../windows/x_walletconfig_w.h"
#include "../core/windowmanager.h"
#include "../core/appcontext.h"
#include "../state/statemachine.h"
#include "../core/global.h"

namespace state {

WalletConfig::WalletConfig( StateContext * context) :
    State (context, STATE::WALLET_CONFIG) {
}

WalletConfig::~WalletConfig() {}

NextStateRespond WalletConfig::execute() {
    if (context->appContext->getActiveWndState() != STATE::WALLET_CONFIG)
        return NextStateRespond(NextStateRespond::RESULT::DONE);

    context->wndManager->switchToWindowEx( mwc::PAGE_X_WALLET_CONFIG,
                new wnd::WalletConfig( context->wndManager->getInWndParent(), this ) );

    return NextStateRespond( NextStateRespond::RESULT::WAIT_FOR_ACTION );
}


wallet::WalletConfig WalletConfig::getWalletConfig() const {
    return context->wallet->getWalletConfig();
}

wallet::WalletConfig WalletConfig::getDefaultWalletConfig() const {
    return context->wallet->getDefaultConfig();
}

bool WalletConfig::setWalletConfig(const wallet::WalletConfig & config) {
    if (context->wallet->setWalletConfig(config)) {
        // restarting the wallet...
        context->stateMachine->executeFrom( STATE::NONE );
        return true;
    }
    return false;
}



core::SendCoinsParams  WalletConfig::getSendCoinsParams() const {
    return context->appContext->getSendCoinsParams();
}

// account refresh will be requested...
void WalletConfig::setSendCoinsParams(const core::SendCoinsParams & params) {
    context->appContext->setSendCoinsParams(params);
    context->wallet->updateWalletBalance(); // Number of outputs might change, requesting update in background
}



}

