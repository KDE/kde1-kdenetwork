################################################################
#
# Infovia at Spain
#
################################################################
name=Spain Infovia

################################################################
# currency settings
################################################################
currency_symbol=PTS
currency_position=right 
currency_digits=2


################################################################
# connection settings
################################################################

per_connection=6.612
minimum_costs=13.224
default=(6.612, 240)

on (monday..friday) between (8:00..21:00) use (6.612, 180)
on (saturday) between (8:00..13:00) use (6.612, 180)

on (1/1) between () use (6.612, 240)
on (12/25) between () use (6.612, 240)
on (easter) between () use (6.612, 240)
on (easter+60) between () use (6.612, 240)
