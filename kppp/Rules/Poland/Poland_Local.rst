################################################################
# This is rule set for Poland local phone cost (up to 25 km).
# Po³±czenia lokalne (do 25 km lub polaczenia do TPNET) koszty.
# Piotr Haberko <gumis@wsi.edu.pl>
# or Piotr Haberko <gumis@gumis.silesia.linux.org.pl>
# updates by Jacek Stolarczyk <jacek@mer.chemia.polsl.gliwice.pl>
# for TP S.A. specific ruleset see TP-Lokalne.rst
################################################################

name=Poland
currency_symbol=PLN
currency_position=right 
currency_digits=2
per_connection=0.0
minimum_costs=0.0

# Therefore the following rule means: "Every 360 seconds 0.29 (with VAT)
# PLN are added to the bill"
# Co 360 sekund dodawane bedzie 29 groszy do rachunku (taryfa nocna).
default=(0.29, 360)
# taryfa dzienna
on (monday..sunday) between (8:00..21:59) use (0.29, 180)

