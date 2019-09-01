import 'package:dart_pdfium/dart_pdfium.dart';
import 'package:test/test.dart';

void main() {
  group('A group of tests', () {
    setUp(() {});

    test('First Test', () {
      InitLibrary();
      var filepath = "testing/hello_world.pdf";
      print("load doc from $filepath");
      var doc = new Document(filepath, null);
      print("doc current Filepath:${doc.filepath}");
      print("doc current Version:${doc.version}");
      print("doc current DocPermissions:${doc.docPermissions}");
      print("doc current SecurityHandlerRevision:${doc.securityHandlerRevision}");
      print("doc current PageCount:${doc.pageCount}");
      var page = doc.loadPage(0);
      print("page current Document:${page.document}");
      print("page current Page:${page.page}");
      print("page current Index:${page.index}");
      print("page current Width:${page.width}");
      print("page current Height:${page.height}");
      print("page current BoundingBox:${page.boundingBox}");
      var bitmap = new Bitmap(768, 1024);
      print("bitmap current Bitmap:${bitmap.bitmap}");
      print("bitmap current Width:${bitmap.width}");
      print("bitmap current Height:${bitmap.height}");
      print("bitmap current Format:${bitmap.format}");
      print("bitmap current Buffer:${bitmap.buffer}");
      print("bitmap current Stride:${bitmap.stride}");
      bitmap.FillRect(0, 0, bitmap.width, bitmap.height, 0xFFFFFFFF);
      page.Render(bitmap, 0, 0, 769, 1024, 0, 0);
      bitmap.Destory();
      page.Close();
      doc.Close();
      DestroyLibrary();
    });
  });
}
