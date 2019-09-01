import 'package:dart_pdfium/dart_pdfium.dart';

main() {
  InitLibrary();
  var doc = new Document("testing/hello_world.pdf", "");
  doc.Close();
  doc.Close();
  DestroyLibrary();
}
