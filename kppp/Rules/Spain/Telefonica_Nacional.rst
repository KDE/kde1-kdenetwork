################################################################
#
# Telefonica at Spain
#
################################################################
name=Spain Telefonica Nacional

################################################################
# currency settings
################################################################
currency_symbol=PTS
currency_position=right 
currency_digits=2


################################################################
# connection settings
################################################################

per_connection=19.836
minimum_costs=26.448
default=(6.612, 20.6)

on (monday..friday) between (8:00..17:00) use (6.612, 7.2)
on (monday..friday) between (17:00..22:00) use (6.612, 10.7)

on (saturday) between (8:00..14:00) use (6.612, 7.2)

on (1/1) between () use (6.612, 20.6)
on (1/6) between () use (6.612, 20.6)
on (5/1) between () use (6.612, 20.6)
on (11/1) between () use (6.612, 20.6)
on (12/6) between () use (6.612, 20.6)
on (12/8) between () use (6.612, 20.6)
on (12/25) between () use (6.612, 20.6)
on (easter) between () use (6.612, 20.6)
on (easter+60) between () use (6.612, 20.6)
