/*******************************************************************************
 The MIT License (MIT)

 Copyright (c) 2014 Alexander Zolotarev <me@alex.bio> from Minsk, Belarus

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 *******************************************************************************/

// Small demo which prints raw (ungzipped) cereal stream from stdin.

// This define is needed to preserve client's timestamps in events.
#define ALOHALYTICS_SERVER
#include "../event_base.h"

#include "../Bricks/rtti/dispatcher.h"

#include <iostream>
#include <iomanip>
#include <typeinfo>

struct Processor {
  void operator()(const AlohalyticsBaseEvent & event) {
    std::cerr << "Unhandled event of type " << typeid(event).name() << std::endl;
  }
  void operator()(const AlohalyticsIdEvent & event) {
  }
  void operator()(const AlohalyticsKeyEvent & event) {
  }
  void operator()(const AlohalyticsKeyValueEvent & event) {
  }
  void operator()(const AlohalyticsKeyPairsEvent & event) {
  }
};

int main(int, char **) {
  cereal::BinaryInputArchive ar(std::cin);
  Processor processor;
  std::string const exc("Failed to read 4 bytes from input stream! Read 0");
  try {
    while (!std::cin.eof()) {
      std::unique_ptr<AlohalyticsBaseEvent> ptr;
      ar(ptr);
      bricks::rtti::RuntimeDispatcher<AlohalyticsBaseEvent,
                                      AlohalyticsKeyPairsEvent,
                                      AlohalyticsIdEvent,
                                      AlohalyticsKeyValueEvent,
                                      AlohalyticsKeyEvent>::DispatchCall(*ptr, processor);
    }
  } catch (const cereal::Exception & ex) {
    if (exc != ex.what()) {
      std::cerr << "Cereal exception: " << ex.what() << std::endl;
    }
  } catch (const bricks::rtti::UnrecognizedPolymorphicType & ex) {
    std::cerr << "UnrecognizedPolymorphicType" << std::endl;
    return -1;
  }
  
  return 0;
}