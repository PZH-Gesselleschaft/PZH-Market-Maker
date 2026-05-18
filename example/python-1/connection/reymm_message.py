# *****************************
# *** Author: Reymon Dev    ***
# *** Date: 01 May of 2026  ***
# *** Update 01 May of 2026 ***
# *** License: MIT          ***
# *****************************

def reymm_message(ws, message):

	# Get Values of json
	json_data = open_json()

	# Obtener Respuesta del servidor
	book_data = json.loads(message)
	print(book_data)

	# Calcular microprecio

	bid_price: float = 0.9999
	ask_price: float = 1.0000

	# Insertar en la base de datos la orden
	cursor.execute("""
		INSERT INTO ORDERS (BID, ASK, SIZE, BID_PRICE, ASK_PRICE, SYMBOL) VALUES (?, ?, ?, ?, ?, ?)
		""",
			(json_data["bid"],
			json_data["ask"],
			json_data["size"],
			bid_price,
			ask_price,
			json_data["symbol"]))

	# ID de la orden
#	cl_ord_id = secrets.token_hex(16)
	reqid = cursor.lastrowid

	ws.send(get_suscrube_balance())
	ws.send(get_suscribe_book())
	ws.send(get_order_bid(bid_price, ask_price))
	ws.send(get_order_ask(bid_price, ask_price))
	
#	if (book_data["asset"] == "USD"):
#		usd_balance = book_data["wallets"]["balance"]
#		print(usd_balance)
#		ws.send(json.dumps(reymm_order_buy))