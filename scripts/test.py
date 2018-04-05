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
#   Test Scenario
#   -------------
#
#    1       2       3
#   ---     ---     ---
#     --Req->
#     <-Rsp--
#     ----------Req->
#     <-Rsp----------
#     <-Ntf-- --Ntf->
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
        self.nodes[1].sendReq(3)
        self.nodes[2].sendNtf()


# Main Routine ---------------------------------------------------------------#
if __name__ == '__main__':
    unittest.main()
