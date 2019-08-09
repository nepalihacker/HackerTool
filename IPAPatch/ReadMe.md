# IPA Patch
IPAPatch provides a simple way to patch iOS Apps, without needing to jailbreak.

# Features
IPAPatch includes an template Xcode project, that provides following features:

* Build & Run third-party ipa with your code injected

You can run your own code inside ipa file as a dynamic library. So you can change behavior of that app by utilizing Objective-C runtime.

* Link external frameworks

* Generate distributable .ipa files

# Instructions
1. Clone or Download This Project

2. Download this project to your local disk

3. Prepare Decrypted IPA File

The IPA file you use need to be decrypted, you can get a decrypted ipa from a jailbroken device or download it directly from an ipa download site, such as http://www.iphonecake.com

4. Replace Placeholder IPA

Replace the IPA file located at IPAPatch/Assets/app.ipa with yours, this is a placeholder file. The filename should remain app.ipa after replacing.
