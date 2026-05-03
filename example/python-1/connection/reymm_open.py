# *****************************
# *** Author: Reymon Dev    ***
# *** Date: 01 May of 2026  ***
# *** Update 01 May of 2026 ***
# *** License: MIT          ***
# *****************************

def reymm_open(ws):

	# Sleep for connect to socket
	conn = sqlite3.connect("transactions.db")
	cursor = conn.cursor()

	# Initialize Global Variables
	json_data = open_json()
	token     = get_signature()["result"]["token"]
	reqid     = 0

	# Create Order Table
	cursor.execute("""

		CREATE TABLE IF NOT EXISTS ORDERS (
			ID INTEGER PRIMARY KEY AUTOINCREMENT,
			BID INTEGER,
			ASK INTEGER,
			SIZE INTEGER,
			BID_PRICE DECIMAL,
			ASK_PRICE DECIMAL,
			SYMBOL VARCHAR(10)
		)

		""")
	conn.commit()

	# Balances
	balances = {
		"method": "subscribe",
		"params": {
			"channel": "balances",
			"token": token
		}
	}

	ws.send(json.dumps(balances))