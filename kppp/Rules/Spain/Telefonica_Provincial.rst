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

per_connection=13.224
minimum_costs=19.836
default=(6.612, 46)

on (monday..friday) between (8:00..17:00) use (6.612, 20)
on (monday..friday) between (17:00..22:00) use (6.612, 22.9)

on (saturday) between (8:00..14:00) use (6.612, 20)

on (1/1) between () use (6.612, 46)
on (1/6) between () use (6.612, 46)
on (5/1) between () use (6.612, 46)
on (11/1) between () use (6.612, 46)
on (12/6) between () use (6.612, 46)
on (12/8) between () use (6.612, 46)
on (12/25) between () use (6.612, 46)
on (easter) between () use (6.612, 46)
on (easter+60) between () use (6.612, 46)
