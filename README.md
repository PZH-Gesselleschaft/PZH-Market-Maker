# Reymon Market Maker

The file used to define what is to be bought or sold is `setting.json`
which must be placed in the same folder as the one where you run the bot

# List Supporterd Exchanges

These are the exchanges that the bot officially supports

1. Binance
2. Kraken

# Dependencies

1. yyjson
2. libcrypto
3. openssl
4. libwebsockets

# Install Dependencies

## Debian

```bash
sudo apt install libyyjson-dev
sudo apt install libcrypto-dev
sudo apt install libopenssl-dev
sudo apt install libwebsockets-dev
```

# Json Format

```json
{
	"exchange":  "",  // Exchange (el nombre es insensible)
	"symbol":    "",  // Símbol donde se va a operar (el nombre es sensible)
	"ordertype": "",  // Tipo de orden (el nombre es sensible)
	
	"bid":  0,    // Cantidad de órdenes en bid (nunca supera al méximo del exchange)
	"ask":  0,    // Cantidad de órdenes en ask (nunca supera al máximo del exchange)
	"size": 0,    // Tamaño de las órdenes
	"gamma": 0.1, // Aversión al riesgo

	"apikey":    "", // API Key
	"apisecret": ""  // API Secret
}
```