program example(input, output);
var x, y, z: integer;
var foo, bar: real;
var baz: real;
function gcd(a, b: integer): integer;
var u: real;
begin
	if b = 0 then gcd := a
	else gcd := gcd(b, a mod b)
end;

function max(a, d: integer; b: real): integer;
begin
	if a > d then max := a
	else max := d
end;

procedure printit(it: integer);
begin
	write(it)
end;

begin
	read(x, y);
	write(gcd(x, y))
end.
