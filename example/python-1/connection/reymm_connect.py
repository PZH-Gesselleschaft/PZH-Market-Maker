# *****************************
# *** Author: Reymon Dev    ***
# *** Date: 01 May of 2026  ***
# *** Update 01 May of 2026 ***
# *** License: MIT          ***
# *****************************

import websocket

def reymm_connect(reymm_url, reymm_open, reymm_message, reymm_error, reymm_close):
	ws = websocket.WebSocketApp(
		reymm_url,
		on_open=reymm_open,
		on_message=reymm_message,
		on_error=reymm_error,
		on_close=reymm_close
	)

	ws.run_forever()