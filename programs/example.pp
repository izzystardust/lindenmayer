program example(input, output);
var x, y, z: integer;
function gcd(a, b: integer): integer;
begin
	if b = 0 then gcd := a
	else gcd := gcd(b, a mod b)
end;

function max(a, d: integer): integer;
begin
	if a > d then max := a
	else max := d
end;

begin
	read(x, y);
	write(gcd(x, y))
end.
