"use strict";
// ********************************
// *** Author: PZH Gesellschaft ***
// *** Date: 13. Mai. 2026      ***
// *** Update 13. Maio. 2026    ***
// *** License: AGPL-3-or-later ***
// ********************************
// *****************
// *** Main Code ***
// *****************
fetch('json/setting.json')
    .then(r => r.text())
    .then(txt => document.getElementById('setting_format').textContent = txt);
fetch('json/exchange.json')
    .then(r => r.text())
    .then(txt => document.getElementById('exchange_format').textContent = txt);
