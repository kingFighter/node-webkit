// Copyright (c) 2012 Intel Corp
// Copyright (c) 2012 The Chromium Authors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy 
// of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell co
// pies of the Software, and to permit persons to whom the Software is furnished
//  to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in al
// l copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IM
// PLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNES
// S FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
//  OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WH
// ETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "content/nw/src/api/shell/shell.h"

#include "base/files/file_path.h"
#include "base/file_util.h"
#include "base/logging.h"
#include "base/values.h"
#include "chrome/browser/platform_util.h"
#include "googleurl/src/gurl.h"

#include "content/nw/src/nw_package.h"
#include "content/nw/src/shell_browser_main_parts.h"
#include "content/nw/src/shell_content_browser_client.h"


namespace api {

// static
void Shell::Call(const std::string& method,
                 const base::ListValue& arguments) {
  if (method == "OpenExternal") {
    std::string uri;
    arguments.GetString(0, &uri);
    platform_util::OpenExternal(GURL(uri));
  } else if (method == "OpenItem") {
    std::string full_path;
    arguments.GetString(0, &full_path);
    platform_util::OpenItem(base::FilePath::FromUTF8Unsafe(full_path));
  } else if (method == "ShowItemInFolder") {
    std::string full_path;
    arguments.GetString(0, &full_path);
    base::FilePath file_path = base::FilePath::FromUTF8Unsafe(full_path);
    
    //change relative path to absolute path
    if (!file_path.IsAbsolute()) {
      content::ShellContentBrowserClient* browser_client =
        static_cast<content::ShellContentBrowserClient*>(content::GetContentClient()->browser());
      base::FilePath package_path = browser_client->shell_browser_main_parts()->package()->path();
      file_util::SetCurrentDirectory(package_path);
      file_path = base::MakeAbsoluteFilePath(file_path);
    }
    DLOG(INFO) << file_path.value().c_str();
    platform_util::ShowItemInFolder(file_path);
  } else {
    NOTREACHED() << "Calling unknown method " << method << " of Shell";
  }
}

}  // namespace api
