################################################################
# This is rule set for Poland local phone cost (up to 25 km).
# Po³±czenia lokalne (do 25 km lub polaczenia do TPNET) koszty.
# Piotr Haberko <gumis@wsi.edu.pl>
# or Piotr Haberko <gumis@gumis.silesia.linux.org.pl>
################################################################

name=Poland
currency_symbol=PLN
currency_position=right 
currency_digits=2
per_connection=0.0
minimum_costs=0.0

# Therefore the following rule means: "Every 180 seconds 0.26 (with VAT)
# PLN are added to the bill"
# Co 180 sekund dodawane bedzie 26 groszy do rachunku.
default=(0.26, 180)
