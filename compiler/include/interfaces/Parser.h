/*
 * Copyright (c) 2021, WSO2 Inc. (http://www.wso2.org) All Rights Reserved.
 *
 * WSO2 Inc. licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef __PARSER__H__
#define __PARSER__H__

#include <cstdint>
#include <ios>

namespace nballerina {

class Parser {
  public:
    virtual uint8_t readU1() = 0;
    virtual int16_t readS2be() = 0;
    virtual int32_t readS4be() = 0;
    virtual int64_t readS8be() = 0;
    virtual double readS8bef() = 0;
    virtual void readChars(char *outBuff, std::streamsize length) = 0;
    virtual void ignore(std::streamsize length) = 0;
};

} // namespace nballerina

#endif //!__PARSER__H__