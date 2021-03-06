/*
 * Copyright (c) 2006-2007 Hyperic, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.hyperic.sigar.vmware;

import java.util.List;

public class VMwareServer extends VMwareObject {
    native void destroy();

    private native void create();

    public native boolean connect(ConnectParams params)
        throws VMwareException;

    public native void disconnect();

    public native boolean isConnected();

    public native boolean isRegistered(String config)
        throws VMwareException;

    public native List getRegisteredVmNames()
        throws VMwareException;

    public native String getResource(String key)
        throws VMwareException;

    public native String exec(String xml)
        throws VMwareException;

    public VMwareServer() {
        create();
    }
}
