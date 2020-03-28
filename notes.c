/* ---------------- ExampleHTML with uncompliant title tags ---------------- */
std::string input = "<!doctype html>\
<html>\
	<head>\
		<title>First title yeah</title>\
		<title>Title2</title>\
	</head>\
	<body>\
		<h1>Text Header</h1>\
	</body>\
</html>\
";
/* ----------------------------- Test Doctypes ----------------------------- */
std::vector<std::vector<char>> doctypes = {
	VectorizeString("<!DOCTYPE html>"),
	VectorizeString("<!DOCTYPE html SYSTEM \"about:legacy-compat\">"),
	VectorizeString("<!DOCTYPE html SYSTEM 'about:legacy-compat'>"),
};
