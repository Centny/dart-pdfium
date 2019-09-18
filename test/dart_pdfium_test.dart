import 'package:dart_pdfium/normal.dart';
import 'package:test/test.dart';

void testAll(String filepath) {
  InitLibrary();
  print("load doc from $filepath");
  var doc = new Document(filepath, null);
  print("doc current native:${doc.nativeObject}");
  print("doc current Filepath:${doc.filepath}");
  print("doc current Version:${doc.version}");
  print("doc current DocPermissions:${doc.docPermissions}");
  print("doc current SecurityHandlerRevision:${doc.securityHandlerRevision}");
  print("doc current PageCount:${doc.pageCount}");
  {
    //test page
    var page = doc.loadPage(0);
    print("page:${page}");
    print("page current native:${page.nativeObject}");
    print("page current Document:${page.document}");
    print("page current Index:${page.index}");
    print("page current Width:${page.width}");
    print("page current Height:${page.height}");
    print("page current BoundingBox:${page.boundingBox}");
    {
      //test bitmap and render
      var bitmap = new Bitmap(768, 1024);
      print("Bitmap:${bitmap}");
      print("Bitmap current native:${bitmap.nativeObject}");
      print("Bitmap current Bitmap:${bitmap.nativeObject}");
      print("Bitmap current Width:${bitmap.width}");
      print("Bitmap current Height:${bitmap.height}");
      print("Bitmap current Format:${bitmap.format}");
      print("Bitmap current Buffer:${bitmap.buffer}");
      print("Bitmap current Stride:${bitmap.stride}");
      bitmap.fillRect(0, 0, bitmap.width, bitmap.height, 0xFFFFFFFF);
      page.render(bitmap, 0, 0, 769, 1024, 0, 0);
      bitmap.destory();
    }
    {
      //test page text
      var text = page.loadText();
      print("PageText:${text}");
      print("PageText current native:${text.nativeObject}");
      print("PageText current page:${text.page}");
      print("PageText current countChars:${text.countChars}");
      print("PageText current getUnicode:${text.getUnicode(0)}");
      print("PageText current getFontSize:${text.getFontSize(0)}");
      print("PageText current getFontInfo:${text.getFontInfo(0)}");
      print("PageText current getCharAngle:${text.getCharAngle(0)}");
      print("PageText current getCharBox:${text.getCharBox(0)}");
      print("PageText current getCharOrigin:${text.getCharOrigin(0)}");
      print("PageText current getCharIndexAtPos:${text.getCharIndexAtPos(100.0, 100.0, 10.0, 10.0)}");
      print("PageText current getText:${text.getText(0, 10)}");
      print("PageText current countRects:${text.countRects(0, 10)}");
      var rect = text.getRect(0);
      print("PageText current getRect:${rect}");
      print("PageText current getBoundedText:${text.getBoundedText(rect)}");
      {
        //test finder
        var finder = text.find("world", finderMatchCase, 0);
        print("Finder:${finder}");
        print("Finder current native:${finder.nativeObject}");
        print("Finder current text:${finder.text}");
        print("Finder current findwhat:${finder.findwhat}");
        print("Finder current flags:${finder.flags}");
        print("Finder current startIndex:${finder.startIndex}");
        print("Finder current next:${finder.next()}");
        print("Finder current resultIndex:${finder.resultIndex}");
        print("Finder current count:${finder.resultCount}");
        print("Finder current next:${finder.next()}");
        print("Finder current resultIndex:${finder.resultIndex}");
        print("Finder current count:${finder.resultCount}");
        print("Finder current prev:${finder.prev()}");
        print("Finder current resultIndex:${finder.resultIndex}");
        print("Finder current count:${finder.resultCount}");
        finder.close();
        print("Finder test done");
      }
      text.close();
      print("PageText test done");
    }
    {
      Bitmap thumbnail = page.loadThumbnail();
      if (thumbnail != null) {
        thumbnail.destory();
      }
    }
    page.close();
    print("Page test done");
  }
  {
    //test load bookmark
    Bookmark bookmark = doc.loadBookmark();
    print("Bookmark:${bookmark}");
    if (bookmark != null) {
      print("Bookmark current native:${bookmark.nativeObject}");
      print("Bookmark current doc:${bookmark.doc}");
      print("Bookmark current title:${bookmark.title}");
      print("Bookmark current pageIndex:${bookmark.pageIndex}");
      Bookmark next = bookmark;
      for (var i = 0; i < 10; i++) {
        if (next != null) {
          print("Bookmark current title:${next.title}");
          print("Bookmark current pageIndex:${next.pageIndex}");
          Bookmark firstChild = next.firstChild;
          if (firstChild != null) {
            print("Bookmark current title:${firstChild.title}");
            print("Bookmark current pageIndex:${firstChild.pageIndex}");
          }
          next = next.next;
        } else {
          break;
        }
      }
    }
    print("Bookmark test done");
  }
  doc.close();
  print("Document test done");
  DestroyLibrary();
}

void main() {
  group('A group of tests', () {
    setUp(() {});

    test('Test Helloworld', () {
      var filepath = "testing/hello_world.pdf";
      testAll(filepath);
      print("testing ${filepath} done\n\n\n\n");
    });

    test('Test Bookmarks', () {
      var filepath = "testing/bookmarks.pdf";
      testAll(filepath);
      print("testing ${filepath} done\n\n\n\n");
      DestroyLibrary();
    });
    test('Test Bookmarks 1', () {
      var filepath = "testing/bookmarks1.pdf";
      testAll(filepath);
      print("testing ${filepath} done\n\n\n\n");
      DestroyLibrary();
    });

    test('Test Error', () {
      InitLibraryWithConfig(0, ".");
      var filepath = "testing/bookmarks.pdf";
      print("load doc from $filepath");
      var doc = new Document(filepath, null);
      new Bitmap(0, 0);
      try {
        //peer error
        DP_CloseDocument(1);
      } catch (e) {
        print(e);
      }
      try {
        //peer error
        DP_CloseDocument(null);
      } catch (e) {
        print(e);
      }
      try {
        //int64 error
        InitLibraryWithConfig(null, null);
      } catch (e) {
        print(e);
      }
      try {
        //cstring error
        DP_LoadDocument(null, "xx");
      } catch (e) {
        print(e);
      }
      try {
        //cstring error
        DP_LoadDocument("xxxx", 11);
      } catch (e) {
        print(e);
      }
      Page page = doc.loadPage(0);
      PageText text = page.loadText();
      try {
        //utf16 string error
        DP_Text_FindStart(text.nativeObject, 1, 0, 0);
      } catch (e) {
        print(e);
      }
      try {
        DP_Text_GetBoundedText(text.nativeObject, null, null, null, null);
      } catch (e) {
        print(e);
      }
      text.close();
      page.close();
      //
      doc.close();
      try {
        new Document("xxxx", null);
      } catch (e) {
        prints(e);
      }
      DestroyLibrary();
    });
  });
}
