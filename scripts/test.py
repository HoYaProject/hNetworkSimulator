#!/usr/bin/env python3
###############################################################################
#   @file       scripts/test.py
#   @brief      Test scripts.
#   @author     llHoYall <hoya128@gmail.com>
#   @version    v1.0
#   @note
#       - 2018.04.02    Created.
###############################################################################
#
#   Scenario
#   --------
#
#   1 -Req-> 2
#   1 <-Rsp- 2
#   1 -Req-> 3
#   1 <-Rsp- 3
#   2 -Ntf-> 1
#   2 -Ntf-> 3
#
###############################################################################

# Import Packages ------------------------------------------------------------#
# Built-In
import unittest
# System
import node


# Test Class -----------------------------------------------------------------#
class ProtocolTest(unittest.TestCase):
    def setUp(self):
        self.nodes = {}
        for i in range(1, 4):
            self.nodes[i] = node.Node(i)

    def tearDown(self):
        for node in list(self.nodes.values()):
            node.stop()
        del self.nodes

    def test(self):
        self.nodes[1].start()
        self.nodes[2].start()
        self.nodes[3].start()
        self.nodes[1].sendReq(2)


# Main Routine ---------------------------------------------------------------#
if __name__ == '__main__':
    unittest.main()
