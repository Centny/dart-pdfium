import 'package:dart_pdfium/normal.dart';
import 'package:image/image.dart';
import 'dart:io';

main() {
  InitLibrary();
  var doc = new Document("testing/bookmarks.pdf", null);
  var page = doc.loadPage(0);
  var bitmap = new Bitmap(page.width.floor() * 2, page.height.floor() * 2, page.transparency);
  bitmap.fillRect(0, 0, bitmap.width, bitmap.height, 0xffffffff);
  page.render(bitmap, 0, 0, bitmap.width, bitmap.height, 0, 0x01);
  var x = Image.fromBytes(
    bitmap.width,
    bitmap.height,
    bitmap.buffer,
  );
  File('example/page0.png')..writeAsBytesSync(encodePng(x));
  DestroyLibrary();
}
