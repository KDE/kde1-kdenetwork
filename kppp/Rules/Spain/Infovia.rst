################################################################
#
# Infovia at Spain
#
# contributed 98/08/18 by Enrique Suárez <enrique@jet.es>
#
################################################################
name=Spain Infovia

################################################################
# currency settings
################################################################
currency_symbol=Pts.
currency_position=right 
currency_digits=2

################################################################
# connection settings
################################################################

# Tafifas sin descuento. Tarificacion por segundos.
# Si no se especifica nada en un dia,
# o tramo orario, se aplicara tarifa nocturna.

default=(0.0317,1)
flat_init_costs=(13.224,160)

on (monday..friday) between (8:00..22:00) use (0.0874, 1)
on (saturday) between (8:00..14:00) use (0.0874, 1)

on (1/1) between () use (0.0317, 1)
on (1/6) between () use (0.0317, 1)
on (12/25) between () use (0.0317, 1)
on (5/1) between () use (0.0317, 1)
on (12/6) between () use (0.0317, 1)
on (12/8) between () use (0.0317, 1)
on (easter) between () use (0.0317, 1)
on (easter+60) between () use (0.0317, 1)
