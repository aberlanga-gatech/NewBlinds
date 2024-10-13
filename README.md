# High-Level Overview:

## Goals:

- Combine mechanical design, computer programming, and circuit design principles to develop a product that satisfies a specific purpose.
- Learn something completely new and unrelated to mostly anything I have done.
- Develop a functional prototype for personal use, and one for demonstrative purposes (to show to recruiters--I'm being fully transparent here).

## Constraints:

- Money doesn't grow on trees--hard limit of $200 out-of-pocket expenses (freebies don't count).
- Time limitations--Ideally have a prototype ready by last week of October (just in time for SHPE)
- Simplicity vs functionality--tradeoff coolness factor with feasibility given my expertise and time constraints.

# The Project

I am making smart blinds for my room.

It is not the coolest thing ever, it is not the most useful thing ever, but it satisfies the high-level goals I set out and there is a clear road apparent for completion.

This project is not "wow, this guy made something that will change the world". That is a nice goal to have overall, but it is not a goal in the scope of this project.

**The goal of this project is to show recruiters that I am able to execute on developing a product from an abstract goal, satisfies the necessary requirements for proper functionality, leverages 3 crucial disciplines in integrated product design, and teaches me something new--all within the constraints of my free time and limited capital.**

## Functional Requirements:

- Must be able to handle the torque load of opening-closing my 6ft blinds (I have a tall roof, picture coming soon).
- Must operate independently from mains power supply. Electricity costs a fortune and I don't need to add to the load.
- Must be able to integrate with Apple HomeKit Access Protocol (HAP) in a light-weight and efficient manner--Python is COMPLETELY out of the question.
- Designed for life--robust mechanical reliability parameters must be defined and DOEs should be implemented to test these requirements.
- ID-Ready: must be aesthetically pleasing (CMF) and adhere to a cohesive design language.

## Other Requirements:

- Must not rely heavily on pre-compiled tools (something like ESP-Zerocode is not allowed)
- Must have a robust codebase--neat, organized, stands on its own. Throwing libraries at the problem makes for very inflexible and hard to troubleshoot code. Not to mention, removes a lot of future customization and learning opportunities.
- (Nice-to-have) lightweight radio communication to de-saturate my WiFi (something like BLE or Thread).

## Project Tracking:

I will set up a project tracking page in the following dates to provide updates on the project as well as my thoughts behind the various design decisions. I will also add the parts I have used/made during development.

I will link it here.

## Those that came before

### Modular Sequencer (status: on hold)

- Bespoke musical production contraption for light-weight music production on the fly
- Integratable with existing frameworks (Eurorack in particular), as well as emphasising custom instruments circuits.
- **Reason For Hold:** MVP goals too ambitious. Complexity in circuit design too high for my current knowledge and time commitment. Will continue to research and learn advanced digital-analog circuit design.
- I dont want to follow a tutorial. While I am OK using tutorials to learn how to implement certain features, this project was far too specific and complex to be developed within my set timeframe. Reducing the complexity and functionality would have resulted in an MVP I wouldn't be satisfied with.
- **Time Spent:** Roughly 4-5 weeks of research, breadboarding, simulating and programming before stoppage.

## Credits & Disclosures:

As much as I would love to say that I developed everything out of first principles, and that I developed everyhting at the register-level, that is both not true and not how things are generally done.

I relied heavily on the following libraries and components:

- HomeSpan (https://github.com/HomeSpan/HomeSpan)
- TMCStepper (https://github.com/teemuatlut/TMCStepper)
