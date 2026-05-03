#!/usr/bin/sh

#!/bin/bash

# ─── Configuración ───────────────────────────────────────────
API_KEY="w4mcIS33KoJzzLWqDmPDWQZHLm3YRYRhfCUpeeDHl5qpi31QVtnVYwjTSOLYEcqP"
SECRET_KEY="Ujz1JdQp4PXDuCmGoMWid5U3EtKh1WKNQMWIjlqB66U2STwjIRhAl7sA7fdTVggg"
BASE_URL="https://demo-api.binance.com"

# ─── Parámetros de la orden ──────────────────────────────────
SYMBOL="USDCUSDT"
SIDE="SELL"
TYPE="LIMIT"
TIME_IN_FORCE="GTC"
QUANTITY="500"
PRICE="1.0001"
TIMESTAMP=$(date +%s%3N)   # milisegundos

# ─── Construir el query string ───────────────────────────────
QUERY="symbol=${SYMBOL}&side=${SIDE}&type=${TYPE}&timeInForce=${TIME_IN_FORCE}&quantity=${QUANTITY}&price=${PRICE}&timestamp=${TIMESTAMP}"

# ─── Firmar con HMAC-SHA256 ──────────────────────────────────
SIGNATURE=$(echo -n "$QUERY" | openssl dgst -sha256 -hmac "$SECRET_KEY" | awk '{print $2}')

# ─── Enviar la orden ─────────────────────────────────────────
curl -s -X POST "${BASE_URL}/api/v3/order" \
  -H "X-MBX-APIKEY: ${API_KEY}" \
  -H "Content-Type: application/x-www-form-urlencoded" \
  -d "${QUERY}&signature=${SIGNATURE}" | jq .