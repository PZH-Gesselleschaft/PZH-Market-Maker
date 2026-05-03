# *****************************
# *** Author: Reymon Dev    ***
# *** Date: 01 May of 2026  ***
# *** Update 01 May of 2026 ***
# *** License: MIT          ***
# *****************************

import time
import hmac
import hashlib
import base64
import requests
import json
import urllib.parse
import sqlite3
import websocket

import reymm.connection.common as common
import reymm.connection.common_funcs as common_funcs

import reymm.connection.reymm_connect as reymm_connect
import reymm.connection.reymm_open as reymm_open
import reymm.connection.reymm_message as reymm_message
import reymm.connection.reymm_error as reymm_error
import reymm.connection.reymm_close as reymm_close

if __name__ == '__main__':

	local_data = None

	with open("setting.json", "r") as fjson:
		local_data = json.load(fjson)

	reymm_connect(
		local_data["url"],
		reymm_open=reymm_open.reymm_open,
		reymm_message=reymm_message.reymm_message,
		reymm_error=reymm_error.reymm_error,
		reymm_close=reymm_close.reymm_close
	)