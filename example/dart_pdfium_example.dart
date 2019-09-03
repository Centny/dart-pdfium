import 'package:dart_pdfium/dart_pdfium.dart';

main() {
  InitLibrary();
  try {
    DP_CloseDocument(1);
  } catch (e) {
    // print(e);
  }
  DestroyLibrary();
}
