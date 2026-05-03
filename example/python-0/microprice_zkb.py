#!/usr/bin/python3

import numpy as np
import pandas as pd
from dataclasses import dataclass
from io import StringIO

# ── 1. DATOS REALES: Zuger Kantonalbank ──────────────────────────────

raw = """Date;Price;Volume
30.04.2026;11600;92
29.04.2026;11450;48
28.04.2026;11650;52
27.04.2026;11600;118
24.04.2026;11550;67
23.04.2026;11450;123
22.04.2026;11350;144
21.04.2026;11250;91
20.04.2026;11050;108
17.04.2026;10950;69
16.04.2026;10900;67
15.04.2026;10850;66
14.04.2026;10800;122
13.04.2026;10900;97
10.04.2026;10650;64
09.04.2026;10650;52
08.04.2026;10500;64
07.04.2026;10300;92
02.04.2026;10250;78
01.04.2026;10200;109
31.03.2026;10250;74
30.03.2026;10350;51
27.03.2026;10250;28
26.03.2026;10450;29
25.03.2026;10450;26
24.03.2026;10450;36
23.03.2026;10450;69
20.03.2026;10450;39
19.03.2026;10350;99
18.03.2026;10500;163
17.03.2026;10450;140
16.03.2026;10150;35
13.03.2026;9980;32
12.03.2026;10050;121
11.03.2026;9980;78
10.03.2026;9880;31
09.03.2026;9880;88
06.03.2026;9940;70
05.03.2026;9880;16
04.03.2026;9940;73
03.03.2026;9840;35
02.03.2026;9860;85
27.02.2026;9860;43
26.02.2026;9840;42
25.02.2026;9820;68
24.02.2026;9880;103
23.02.2026;9840;37
20.02.2026;9940;81
19.02.2026;9900;55
18.02.2026;9800;17
17.02.2026;9820;80
16.02.2026;9820;27
13.02.2026;9860;92
12.02.2026;9800;52
11.02.2026;9780;36
10.02.2026;9720;36
09.02.2026;9720;50
06.02.2026;9600;56
05.02.2026;9680;27
04.02.2026;9660;42
03.02.2026;9580;82
02.02.2026;9500;51
30.01.2026;9400;58
29.01.2026;9320;32
28.01.2026;9360;29
27.01.2026;9400;44
26.01.2026;9340;47
23.01.2026;9300;89
22.01.2026;9300;61
21.01.2026;9260;70
20.01.2026;9220;38
19.01.2026;9220;59
16.01.2026;9300;58
15.01.2026;9200;49
14.01.2026;9100;49
13.01.2026;9060;73
12.01.2026;9040;64
09.01.2026;9040;65
08.01.2026;9000;77
07.01.2026;8940;55
06.01.2026;8940;25
05.01.2026;8940;97
30.12.2025;8840;29
29.12.2025;8900;33
23.12.2025;8860;53
22.12.2025;8860;59
19.12.2025;8820;75
18.12.2025;8900;66
17.12.2025;8800;42
16.12.2025;8820;109
15.12.2025;8780;74
12.12.2025;8800;15
11.12.2025;8800;55
10.12.2025;8740;49
09.12.2025;8800;46
08.12.2025;8800;77
05.12.2025;8740;30
04.12.2025;8780;36
03.12.2025;8800;42
02.12.2025;8860;70
01.12.2025;8880;49
28.11.2025;8860;66
27.11.2025;8800;68
26.11.2025;8800;27
25.11.2025;8800;34
24.11.2025;8800;15
21.11.2025;8820;53
20.11.2025;8660;52"""

df = pd.read_csv(StringIO(raw), sep=';')
df['Date'] = pd.to_datetime(df['Date'], format='%d.%m.%Y')
df = df.sort_values('Date').reset_index(drop=True)

# ── 2. CONSTRUIR SNAPSHOTS SINTÉTICOS DE ORDER BOOK ──────────────────
# Con datos diarios (precio + volumen) no tenemos bid/ask separados.
# Reconstruimos un book sintético realista:
#   - Spread típico para acciones suizas ilíquidas: ~0.3-0.6% del precio
#   - Imbalance ∝ dirección del movimiento reciente (momentum)

@dataclass
class OB:
    bid_price: float
    ask_price: float
    bid_size:  float
    ask_size:  float

def simulate_book(df):
    """
    Genera un order book sintético por sesión.
    Lógica:
      - spread = 0.4% del precio (típico acción ilíquida suiza)
      - imbalance depende del retorno vs. media móvil:
        retorno positivo → más presión compradora (I > 0.5)
    """
    records = []
    prices  = df['Price'].values
    volumes = df['Volume'].values
    n = len(prices)
    
    spread_pct = 0.004  # 0.4% spread típico
    
    for i in range(n):
        p = prices[i]
        v = volumes[i]
        half_spread = p * spread_pct / 2
        
        # Imbalance: momentum de los últimos 3 días
        if i >= 3:
            ret_3d = (prices[i] - prices[i-3]) / prices[i-3]
            # Escalar retorno → imbalance ∈ [0.2, 0.8]
            I = 0.5 + np.clip(ret_3d * 30, -0.3, 0.3)
        else:
            I = 0.5
        
        bid_p = round(p - half_spread, 2)
        ask_p = round(p + half_spread, 2)
        # Distribuir volumen según imbalance
        bid_s = round(v * I, 1)
        ask_s = round(v * (1 - I), 1)
        
        records.append(OB(bid_p, ask_p, bid_s, ask_s))
    
    return records

books = simulate_book(df)

# ── 3. MICRO-PRECIO ───────────────────────────────────────────────────

def mid_price(ob):    return (ob.bid_price + ob.ask_price) / 2
def imbalance(ob):
    t = ob.bid_size + ob.ask_size
    return ob.bid_size / t if t > 0 else 0.5
def spread(ob):       return ob.ask_price - ob.bid_price
def weighted_mid(ob):
    I = imbalance(ob)
    return I * ob.ask_price + (1 - I) * ob.bid_price
def micro_price(ob):
    return mid_price(ob) + (imbalance(ob) - 0.5) * spread(ob)

# ── 4. CALCULAR TODAS LAS SERIES ─────────────────────────────────────

results = []
for i, (row, ob) in enumerate(zip(df.itertuples(), books)):
    M  = mid_price(ob)
    I  = imbalance(ob)
    S  = spread(ob)
    WM = weighted_mid(ob)
    MP = micro_price(ob)
    g  = MP - M
    
    # Quotes del market maker con spread 0.6%
    half_mm = row.Price * 0.003
    mm_bid  = MP - half_mm
    mm_ask  = MP + half_mm
    
    results.append({
        'date':        row.Date.strftime('%d.%m.%Y'),
        'price':       row.Price,
        'volume':      row.Volume,
        'bid':         ob.bid_price,
        'ask':         ob.ask_price,
        'mid':         round(M, 2),
        'imbalance':   round(I, 4),
        'spread_chf':  round(S, 2),
        'spread_pct':  round(S / M * 100, 3),
        'weighted_mid': round(WM, 2),
        'micro_price': round(MP, 2),
        'g_adj':       round(g, 2),
        'mm_bid':      round(mm_bid, 2),
        'mm_ask':      round(mm_ask, 2),
    })

res = pd.DataFrame(results)

# ── 5. ESTADÍSTICAS ───────────────────────────────────────────────────
print("=" * 65)
print("ZUGER KANTONALBANK (CH0493891243) — Micro-Price Analysis")
print("=" * 65)
print(f"Período:      {res['date'].iloc[0]} → {res['date'].iloc[-1]}")
print(f"Observaciones: {len(res)}")
print(f"Precio inicial: CHF {res['price'].iloc[0]:,.0f}")
print(f"Precio final:   CHF {res['price'].iloc[-1]:,.0f}")
print(f"Retorno total:  {(res['price'].iloc[-1]/res['price'].iloc[0]-1)*100:.1f}%")
print(f"Volumen medio:  {res['volume'].mean():.0f} acciones/día")
print()
print("── Métricas del micro-precio ──────────────────────────────────")
print(f"Spread medio:   CHF {res['spread_chf'].mean():.1f} ({res['spread_pct'].mean():.3f}%)")
print(f"Imbalance medio: {res['imbalance'].mean():.3f}")
print(f"Ajuste g medio: CHF {res['g_adj'].mean():.1f}")
print(f"Diferencia MP vs Mid (media): {(res['micro_price'] - res['mid']).mean():.2f} CHF")
print()
print("── Últimas 5 sesiones ─────────────────────────────────────────")
cols = ['date','price','imbalance','mid','weighted_mid','micro_price','g_adj','mm_bid','mm_ask']
print(res[cols].tail(5).to_string(index=False))

res.to_csv('/home/reymon/Documentos/projects/reymon/reymm/zkb_microprice.csv', index=False)
print("\n✓ Guardado: zkb_microprice.csv")
