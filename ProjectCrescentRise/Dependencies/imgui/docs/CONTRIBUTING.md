# Contributing Guidelines

## Index

- [Getting Started & General Advice](#getting-started--general-advice)
- [Issues vs Discussions](#issues-vs-discussions)
- [How to open an Issue](#how-to-open-an-issue)
- [How to open a Pull Request](#how-to-open-a-pull-request)
- [Copyright / Contributor License Agreement](#copyright--contributor-license-agreement)

## Getting Started & General Advice

- Article: [How To Ask Good Questions](https:
- Please browse the [Wiki](https:
- Please read [Getting Started](https:
- Please read [docs/FAQ.md](https:
- Please read [docs/FONTS.md](https:
- Please run `ImGui::ShowDemoWindow()` to explore the demo and its sources.
- Please use the search function of your IDE to search for symbols and comments related to your situation.
- Please use the search function of GitHub to look for similar topics (always include 'Closed' issues/pr in your search).
- You may [browse issues by Labels](https:
- Please use a web search engine to look for similar issues.
- If you get a crash or assert, use a debugger to locate the line triggering it and read the comments around.
- Please don't be a [Help Vampire](https:

## 'Issues' vs 'Discussions'

We are happy to use 'Issues' for many type of open-ended questions. We are encouraging 'Issues' becoming an enormous, centralized and cross-referenced database of Dear ImGui contents.

Only if you:
- Cannot BUILD or LINK examples.
- Cannot BUILD, or LINK, or RUN Dear ImGui in your application or custom engine.
- Cannot LOAD a font.

Then please [use the Discussions forums](https:

If Dear ImGui is successfully showing in your app and you have used Dear ImGui before, you can open an Issue. Any form of discussions is welcome as a new issue.

## How to open an issue

You may use the Issue Tracker to submit bug reports, feature requests or suggestions. You may ask for help or advice as well. But **PLEASE CAREFULLY READ THIS WALL OF TEXT. ISSUES IGNORING THOSE GUIDELINES MAY BE CLOSED. USERS IGNORING THOSE GUIDELINES MIGHT BE BLOCKED.**

Please do your best to clarify your request. The amount of incomplete or ambiguous requests due to people not following those guidelines is often overwhelming. Issues created without the requested information may be closed prematurely. Exceptionally entitled, impolite, or lazy requests may lead to bans.

**PLEASE UNDERSTAND THAT OPEN-SOURCE SOFTWARE LIVES OR DIES BY THE AMOUNT OF ENERGY MAINTAINERS CAN SPARE. WE HAVE LOTS OF STUFF TO DO. THIS IS AN ATTENTION ECONOMY AND MANY LAZY OR MINOR ISSUES ARE HOGGING OUR ATTENTION AND DRAINING ENERGY, TAKING US AWAY FROM MORE IMPORTANT WORK.**

Steps:

- Article: [How To Ask Good Questions](https:
- **PLEASE DO FILL THE REQUESTED NEW ISSUE TEMPLATE.** Including Dear ImGui version number, branch name, platform/renderer back-ends (imgui_impl_XXX files), operating system.
- **Try to be explicit with your GOALS, your EXPECTATIONS and what you have tried**.  Be mindful of [The XY Problem](http:
- **Please INCLUDE CODE. Provide a Minimal, Complete, and Verifiable Example ([MCVE](https:
- **Attach screenshots (or GIF/video) to clarify the context**. They often convey useful information that is omitted by the description. You can drag pictures/files in the message edit box. Avoid using 3rd party image hosting services, prefer the long-term longevity of GitHub attachments (you can drag pictures into your post). On Windows, you can use [ScreenToGif](https:
- **If you are discussing an assert or a crash, please provide a debugger callstack**. Never state "it crashes" without additional information. If you don't know how to use a debugger and retrieve a callstack, learning about it will be useful.
- **Please make sure that your project has asserts enabled.** Calls to IM_ASSERT() are scattered in the code to help catch common issues. When an assert is triggered read the comments around it. By default IM_ASSERT() calls the standard assert() function. To verify that your asserts are enabled, add the line `IM_ASSERT(false);` in your main() function. Your application should display an error message and abort. If your application doesn't report an error, your asserts are disabled.
- Please state if you have made substantial modifications to your copy of Dear ImGui or the back-end.
- If you are not calling Dear ImGui directly from C++, please provide information about your Language and the wrapper/binding you are using.
- Be mindful that messages are being sent to the mailbox of "Watching" users. Try to proofread your messages before sending them. Edits are not seen by those users unless they browse the site.

**Some unfortunate words of warning**
- If you are involved in cheating schemes (e.g. DLL injection) for competitive online multiplayer games, please don't post here. We won't answer and you will be blocked. It doesn't matter if your question relates to said project. We've had too many of you and need to protect our time and sanity.
- Due to frequent abuse of this service from the aforementioned users, if your GitHub account is anonymous and was created five minutes ago please understand that your post will receive more scrutiny and incomplete questions will be harshly dismissed.

If you have been using Dear ImGui for a while or have been using C/C++ for several years or have demonstrated good behavior here, it is ok to not fulfill every item to the letter. Those are guidelines and experienced users or members of the community will know which information is useful in a given context.

## How to open a Pull Request

- **Please understand that by submitting a PR you are also submitting a request for the maintainer to review your code and then take over its maintenance.** PR should be crafted both in the interest of the end-users and also to ease the maintainer into understanding and accepting it.
- Many PRs are useful to demonstrate a need and a possible solution but aren't adequate for merging (causing other issues, not seeing other aspects of the big picture, etc.). In doubt, don't hesitate to push a PR because that is always the first step toward pointing toward a problem, and finding the mergeable solution! Even if a PR stays unmerged for a long time, its presence can be useful for other users and helps toward finding a general solution.
- **When adding a feature,** please describe the usage context (how you intend to use it, why you need it, etc.). Be mindful of [The XY Problem](http:
- **When fixing a warning or compilation problem,** please post the compiler log and specify the compiler version and platform you are using.
- **Attach screenshots (or GIF/video) to clarify the context and demonstrate the feature at a glance.** You can drag pictures/files in the message edit box. Prefer the long-term longevity of GitHub attachments over 3rd party hosting (you can drag pictures into your post).
- **Make sure your code follows the coding style already used in the codebase:** 4 spaces indentations (no tabs), `local_variable`, `FunctionName()`, `MemberName`, `
- **Make sure you create a branch dedicated to the pull request**. In Git, 1 PR is associated to 1 branch. If you keep pushing to the same branch after you submitted the PR, your new commits will appear in the PR (we can still cherry-pick individual commits).

## Copyright / Contributor License Agreement

Any code you submit will become part of the repository and be distributed under the [Dear ImGui license](https:

You also agree by submitting your code that you grant all transferable rights to the code to the project maintainer, including for example re-licensing the code, modifying the code, and distributing it in source or binary forms. Specifically, this includes a requirement that you assign copyright to the project maintainer. For this reason, do not modify any copyright statements in files in any PRs.

