program factorial(input, output);
var a: integer;
function factorial(a: integer): integer;
begin
	if a < 1 then factorial := 1
	else factorial := a * factorial(a - 1)
end;

begin
	read(a);
	write(factorial(a))
end.
