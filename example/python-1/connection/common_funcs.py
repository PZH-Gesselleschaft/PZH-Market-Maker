# *****************************
# *** Author: Reymon Dev    ***
# *** Date: 01 May of 2026  ***
# *** Update 01 May of 2026 ***
# *** License: MIT          ***
# *****************************

# **********************
# *** Json Functions ***
# **********************

def open_json():
    with open("setting.json", "r") as fjson:
        return json.load(fjson)

# ************************
# *** Crypto Functions ***
# ************************

def get_signature():

    url_path = "/0/private/GetWebSocketsToken"
    api_url = "https://api.kraken.com" + url_path
    
    nonce = int(time.time() * 1000)
    data = {"nonce": nonce}

    post_data = urllib.parse.urlencode(data)
    encoded = (str(nonce) + post_data).encode()
    message = url_path.encode() + hashlib.sha256(encoded).digest()
    
    signature = hmac.new(
        base64.b64decode(json_data["apisecret"]),
        message,
        hashlib.sha512
    )
    
    headers = {
        "API-Key": json_data["apikey"],
        "API-Sign": base64.b64encode(signature.digest()).decode()
    }
    
    resp = requests.post(api_url, headers=headers, data=data)
    return resp.json()

# *************************
# *** Channel Functions ***
# *************************

def get_suscribe_book():

    channel_book = {
        "method": "subscribe",
        "params": {
            "channel": "book",
            "symbol": json_data["symbol"],
            "depth": 100
        }
    }

    return json.dumps(channel_book)

def get_suscrube_balance():

    balances = {
        "method": "subscribe",
        "params": {
            "channel": "balances",
            "snapshot": False,
            "token": token
        }
    }

    return json.dumps(balances)

# ***********************
# *** Order Functions ***
# ***********************

def get_order_bid(bid_price, ask_price):

    reymm_order_bid = {
        
        "method": "add_order",
        "params": {
            "order_type": json_data["ordertype"],
            "side": "buy",
            "limit_price": bid_price,
            "order_qty": json_data["size"],
            "symbol": json_data["symbol"],
            "token": token,
            "conditional": {
                "order_type": "take-profit-limit",
                "limit_price": ask_price,
                "trigger_price": ask_price
            }
        },

        "req_id": reqid
    }

    return json.dumps(reymm_order_bid)

def get_order_ask(bid_price, ask_price):

    reymm_order_ask = {
        
        "method": "add_order",
        "params": {
            "order_type": json_data["ordertype"],
            "side": "sell",
            "limit_price": ask_price,
            "order_qty": json_data["size"],
            "symbol": json_data["symbol"],
            "token": token,
            "conditional": {
                "order_type": "take-profit-limit",
                "limit_price": bid_price,
                "trigger_price": bid_price
            }
        },

        "req_id": reqid
    }

    return json.dumps(reymm_order_ask)