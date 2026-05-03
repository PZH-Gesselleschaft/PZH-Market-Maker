def micro(bid:float, ask:float, bid_size, ask_size) -> float:
	M:float = (bid + ask) / 2
	S:float = ask - bid
	I:float = bid_size() / (bid_size() + ask_size())
	G:float = (I - 0.5) * S
	return M + G