########## GEMINI⾏情接⼝ ########## ## https://api.gemini.com/v1/pubticker/:symbol
import requests
import json

gemini_ticker = 'https://api.gemini.con/v1/pubticker/{}'
symbol = 'btcusd'
btc_data = requests.get(gemini_ticker.format(symbol)).json()
print(json.dumps(btc_data, indent =4))

