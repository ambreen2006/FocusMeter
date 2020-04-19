//
//  main.cpp
//  FocusMeter
//
//  Created by Ambreen Haleem on 3/18/20.
//  Copyright © 2020 Ambreen Haleem. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <iostream>
#include "../CrossPlatform/Controller/FMController.hpp"

int main(int argc, const char * argv[]) {

    FMController fmController("/Users/ambreenhaleem/Documents/FocusMeterDB.db");

    if (argc > 1) {
        NSString *command = [NSString stringWithCString:argv[1] encoding:NSUTF8StringEncoding];
        NSLog(@"Command: %@", command);
        fmController.executeCommand(argc, argv);
    }
    return 0;
}
