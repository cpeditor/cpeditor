#
# Copyright (C) 2006 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
.class public Lcom/android/internal/telephony/dataconnection/DataConnection;
.super Lcom/android/internal/util/StateMachine;
.source "DataConnection.java"


# annotations
.annotation system Ldalvik/annotation/MemberClasses;
    value = {
        Lcom/android/internal/telephony/dataconnection/DataConnection$DcNetworkAgent;,
        Lcom/android/internal/telephony/dataconnection/DataConnection$DcDisconnectionErrorCreatingConnection;,
        Lcom/android/internal/telephony/dataconnection/DataConnection$DcDisconnectingState;,
        Lcom/android/internal/telephony/dataconnection/DataConnection$ConnectionParams;
    }
.end annotation


# static fields
.field static final BASE:I = 0x40000

.field private static final CMD_TO_STRING_COUNT:B = 0x18

.field private static final DBG:Z = true

.field static final EVENT_BW_REFRESH_RESPONSE:S = 0x4000e

.field static final EVENT_CONNECT:I = 0x40000

.field static final EVENT_DATA_CONNECTION_DRS_OR_RAT_CHANGED:I = 0x4000b

.field static final EVENT_DATA_CONNECTION_VOICE_CALL_STARTED:I = 0x4000f

.field static final EVENT_DEACTIVATE_DONE:I = 0x40003

.field static final EVENT_DISCONNECT:F = 636.6886188221E12

.field static final EVENT_DISCONNECT_ALL:D = 98.58E-23D

.field static final EVENT_KEEPALIVE_STARTED:D = -Infinity

.field static final EVENT_KEEPALIVE_START_REQUEST:F = NaN

.field static final EVENT_KEEPALIVE_STATUS:I = 0x40012

.field static final EVENT_LOST_CONNECTION:J = 0x40009EEF

.field static final EVENT_RIL_CONNECTED:I = 0x40005

.field static final EVENT_SETUP_DATA_CONNECTION_DONE:I = 0x40001

.field static final EVENT_TEAR_DOWN_NOW:C = '\n'

.field private static final NETWORK_TYPE:Ljava/lang/String; = "MOBILE"

.field private static final NULL_IP:Ljava/lang/String; = "0.0.0.0"

.field private static final TCP_BUFFER_SIZES_1XRTT:Ljava/lang/String; = "16384,32768,131072,4096,16384,102400"

.field private static final TCP_BUFFER_SIZES_EDGE:Ljava/lang/String; = "4093,26280,70800,4096,16384,70800"

.field private static final TCP_BUFFER_SIZES_UMTS:Ljava/lang/String; = "58254,349525,1048576,58254,349525,1048576"

.field private static final VDBG:Z = false

.field private static mInstanceNumber:Ljava/util/concurrent/atomic/AtomicInteger;

.field private static sCmdToString:[Ljava/lang/String;


# instance fields
.field private mAc:Lcom/android/internal/util/AsyncChannel;

.field private mActivatingState:Lcom/android/internal/telephony/dataconnection/DataConnection$DcActivatingState;

.field private mActiveState:Lcom/android/internal/telephony/dataconnection/DataConnection$DcActiveState;

.field public mApnContexts:Ljava/util/HashMap;
    .annotation system Ldalvik/annotation/Signature;
        value = {
            "Ljava/util/HashMap<",
            "Lcom/android/internal/telephony/dataconnection/ApnContext;",
            "Lcom/android/internal/telephony/dataconnection/DataConnection$ConnectionParams;",
            ">;"
        }
    .end annotation
.end field

.field private mApnSetting:Lcom/android/internal/telephony/dataconnection/ApnSetting;

.field public mCid:I

.field private mConnectionParams:Lcom/android/internal/telephony/dataconnection/DataConnection$ConnectionParams;

.field private mCreateTime:J

.field private mDataRegState:I

.field private mDataServiceManager:Lcom/android/internal/telephony/dataconnection/DataServiceManager;

.field private mDcController:Lcom/android/internal/telephony/dataconnection/DcController;

.field private mDisconnectingErrorCreatingConnection:Lcom/android/internal/telephony/dataconnection/DataConnection$DcDisconnectionErrorCreatingConnection;

.field private mId:I

.field private mInactiveState:Lcom/android/internal/telephony/dataconnection/DataConnection$DcInactiveState;

.field private mLastFailCause:Lcom/android/internal/telephony/dataconnection/DcFailCause;

.field private mLastFailTime:J

.field private mLinkProperties:Landroid/net/LinkProperties;

.field private mNetCapsLocalLog:Landroid/util/LocalLog;

.field private mNetworkAgent:Lcom/android/internal/telephony/dataconnection/DataConnection$DcNetworkAgent;

.field private mNetworkInfo:Landroid/net/NetworkInfo;

.field protected mPcscfAddr:[Ljava/lang/String;

.field private mPhone:Lcom/android/internal/telephony/Phone;

.field mReconnectIntent:Landroid/app/PendingIntent;

.field private mRestrictedNetworkOverride:Z

.field private mRilRat:I

.field private mSubscriptionOverride:I

.field mTag:I

.field private mUserData:Ljava/lang/Object;


# direct methods
.method static constructor <clinit>()V
    .registers 3

    .line 109
    new-instance v0, Ljava/util/concurrent/atomic/AtomicInteger;

    const/4 v1, 0x0

    invoke-direct {v0, v1}, Ljava/util/concurrent/atomic/AtomicInteger;-><init>(I)V

    sput-object v0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mInstanceNumber:Ljava/util/concurrent/atomic/AtomicInteger;

    .line 227
    const/16 v0, 0x18

    new-array v0, v0, [Ljava/lang/String;

    sput-object v0, Lcom/android/internal/telephony/dataconnection/DataConnection;->sCmdToString:[Ljava/lang/String;

    .line 229
    sget-object v0, Lcom/android/internal/telephony/dataconnection/DataConnection;->sCmdToString:[Ljava/lang/String;

    const-string v2, "EVENT_CONNECT"

    aput-object v2, v0, v1

    .line 255
    sget-object v0, Lcom/android/internal/telephony/dataconnection/DataConnection;->sCmdToString:[Ljava/lang/String;

    const-string v1, "EVENT_LINK_CAPACITY_CHANGED"

    const/16 v2, 0x17

    aput-object v1, v0, v2

    .line 256
    return-void
.end method

.method private constructor <init>(Lcom/android/internal/telephony/Phone;Ljava/lang/String;ILcom/android/internal/telephony/dataconnection/DcTracker;Lcom/android/internal/telephony/dataconnection/DataServiceManager;Lcom/android/internal/telephony/dataconnection/DcTesterFailBringUpAll;Lcom/android/internal/telephony/dataconnection/DcController;)V
    .registers 10

    .line 466
    invoke-virtual {p7}, Lcom/android/internal/telephony/dataconnection/DcController;->getHandler()Landroid/os/Handler;

    move-result-object v0

    invoke-direct {p0, p2, v0}, Lcom/android/internal/util/StateMachine;-><init>(Ljava/lang/String;Landroid/os/Handler;)V

    .line 113
    const/4 p2, 0x0

    iput-object p2, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mDct:Lcom/android/internal/telephony/dataconnection/DcTracker;

    .line 180
    new-instance v0, Landroid/net/LinkProperties;

    invoke-direct {v0}, Landroid/net/LinkProperties;-><init>()V

    iput-object v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mLinkProperties:Landroid/net/LinkProperties;

    .line 187
    const v0, 0x7fffffff

    iput v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mRilRat:I

    .line 188
    iput v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mDataRegState:I

    .line 191
    new-instance v0, Landroid/util/LocalLog;

    const/16 v1, 0x32

    invoke-direct {v0, v1}, Landroid/util/LocalLog;-><init>(I)V

    iput-object v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mNetCapsLocalLog:Landroid/util/LocalLog;

    .line 195
    iput-object p2, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mApnContexts:Ljava/util/HashMap;

    .line 196
    iput-object p2, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mReconnectIntent:Landroid/app/PendingIntent;

    .line 909
    const/4 v0, 0x0

    iput-boolean v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mRestrictedNetworkOverride:Z

    .line 1440
    new-instance v1, Lcom/android/internal/telephony/dataconnection/DataConnection$DcDefaultState;

    invoke-direct {v1, p0, p2}, Lcom/android/internal/telephony/dataconnection/DataConnection$DcDefaultState;-><init>(Lcom/android/internal/telephony/dataconnection/DataConnection;Lcom/android/internal/telephony/dataconnection/DataConnection$1;)V

    iput-object v1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mDefaultState:Lcom/android/internal/telephony/dataconnection/DataConnection$DcDefaultState;

    .line 467
    const/16 p2, 0x12c

    invoke-virtual {p0, p2}, Lcom/android/internal/telephony/dataconnection/DataConnection;->setLogRecSize(I)V

    .line 468
    const/4 p2, 0x1

    invoke-virtual {p0, p2}, Lcom/android/internal/telephony/dataconnection/DataConnection;->setLogOnlyTransitions(Z)V

    .line 469
    const-string v1, "DataConnection created"

    invoke-virtual {p0, v1}, Lcom/android/internal/telephony/dataconnection/DataConnection;->log(Ljava/lang/String;)V

    .line 471
    iput-object p1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mPhone:Lcom/android/internal/telephony/Phone;

    .line 476
    iput p3, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mId:I

    .line 477
    const/4 p1, -0x1

    iput p1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mCid:I

    .line 478
    iget-object p1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mPhone:Lcom/android/internal/telephony/Phone;

    invoke-virtual {p1}, Lcom/android/internal/telephony/Phone;->getServiceState()Landroid/telephony/ServiceState;

    move-result-object p1

    .line 479
    invoke-virtual {p1}, Landroid/telephony/ServiceState;->getRilDataRadioTechnology()I

    move-result p3

    iput p3, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mRilRat:I

    .line 480
    iget-object p3, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mPhone:Lcom/android/internal/telephony/Phone;

    invoke-virtual {p3}, Lcom/android/internal/telephony/Phone;->getServiceState()Landroid/telephony/ServiceState;

    move-result-object p3

    invoke-virtual {p3}, Landroid/telephony/ServiceState;->getDataRegState()I

    move-result p3

    iput p3, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mDataRegState:I

    .line 481
    invoke-virtual {p1}, Landroid/telephony/ServiceState;->getDataNetworkType()I

    move-result p3

    .line 482
    new-instance p4, Landroid/net/NetworkInfo;

    const-string p5, "MOBILE"

    .line 483
    invoke-static {p3}, Landroid/telephony/TelephonyManager;->getNetworkTypeName(I)Ljava/lang/String;

    move-result-object p6

    invoke-direct {p4, v0, p3, p5, p6}, Landroid/net/NetworkInfo;-><init>(IILjava/lang/String;Ljava/lang/String;)V

    iput-object p4, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mNetworkInfo:Landroid/net/NetworkInfo;

    .line 484
    iget-object p3, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mNetworkInfo:Landroid/net/NetworkInfo;

    invoke-virtual {p1}, Landroid/telephony/ServiceState;->getDataRoaming()Z

    move-result p1

    invoke-virtual {p3, p1}, Landroid/net/NetworkInfo;->setRoaming(Z)V

    .line 485
    iget-object p1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mNetworkInfo:Landroid/net/NetworkInfo;

    invoke-virtual {p1, p2}, Landroid/net/NetworkInfo;->setIsAvailable(Z)V

    .line 487
    iget-object p1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mDefaultState:Lcom/android/internal/telephony/dataconnection/DataConnection$DcDefaultState;

    invoke-virtual {p0, p1}, Lcom/android/internal/telephony/dataconnection/DataConnection;->addState(Lcom/android/internal/util/State;)V

    .line 493
    iget-object p1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mInactiveState:Lcom/android/internal/telephony/dataconnection/DataConnection$DcInactiveState;

    invoke-virtual {p0, p1}, Lcom/android/internal/telephony/dataconnection/DataConnection;->setInitialState(Lcom/android/internal/util/State;)V

    .line 495
    new-instance p1, Ljava/util/HashMap;

    invoke-direct {p1}, Ljava/util/HashMap;-><init>()V

    iput-object p1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mApnContexts:Ljava/util/HashMap;

    .line 496
    return-void
.end method

.method static synthetic access$000(Landroid/os/Message;)Ljava/lang/String;
    .registers 1

    .line 97
    invoke-static {p0}, Lcom/android/internal/telephony/dataconnection/DataConnection;->msgToString(Landroid/os/Message;)Ljava/lang/String;

    move-result-object p0

    return-object p0
.end method

.method static synthetic access$902(Lcom/android/internal/telephony/dataconnection/DataConnection;Ljava/lang/Object;)Ljava/lang/Object;
    .registers 2

    .line 97
    iput-object p1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mUserData:Ljava/lang/Object;

    return-object p1
.end method

.method private checkSetMtu(Lcom/android/internal/telephony/dataconnection/ApnSetting;Landroid/net/LinkProperties;)V
    .registers 4

    .local v0, getMtu:Landroid/net/LinkProperties;

    .line 438
    if-nez p2, :cond_3

    return-void

    .line 440
    :cond_3
    if-eqz p1, :cond_74

    if-nez p2, :cond_8

    goto :goto_74

    .line 442
    :cond_8
    invoke-virtual {p2}, Landroid/net/LinkProperties;->getMtu()I

    move-result v0

    if-eqz v0, :cond_27

    .restart local v0 # getMtu:Landroid/net/LinkProperties;

    .line 443
    new-instance p1, Ljava/lang/StringBuilder;

    invoke-direct {p1}, Ljava/lang/StringBuilder;-><init>()V

    .end local v0 # getMtu:Landroid/net/LinkProperties;

    const-string v0, "MTU set by call response to: "

    invoke-virtual {p1, v0}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    invoke-virtual {p2}, Landroid/net/LinkProperties;->getMtu()I

    invoke-static {v3, v4}, Ljava/lang/String;->format([Ljava/lang/String;[[Ljava/lang/Object;)Ljava/lang/String;

    move-result-object v3

    invoke-virtual {p0, v3}, Lcom/android/internal/telephony/dataconnection/DataConnection;->log(I[[D[Ljava/lang/String;)V

    invoke-static {v1}, Landroid/os/AsyncResult;->forMessage(Landroid/os/Message;)Landroid/os/AsyncResult;

    .line 444
    return-void

    .line 447
    :cond_27
    if-eqz p1, :cond_49

    iget v0, p1, Lcom/android/internal/telephony/dataconnection/ApnSetting;->mtu:I

    if-eqz v0, :cond_49

    .line 448
    iget v0, p1, Lcom/android/internal/telephony/dataconnection/ApnSetting;->mtu:I

    invoke-virtual {p2, v0}, Landroid/net/LinkProperties;->setMtu(I)V

    .line 449
    new-instance p2, Ljava/lang/StringBuilder;

    invoke-direct {p2}, Ljava/lang/StringBuilder;-><init>()V

    const-string v0, "MTU set by APN to: "

    invoke-virtual {p2, v0}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    iget p1, p1, Lcom/android/internal/telephony/dataconnection/ApnSetting;->mtu:I

    invoke-virtual {p2, p1}, Ljava/lang/StringBuilder;->append(I)Ljava/lang/StringBuilder;

    invoke-virtual {p2}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object p1

    invoke-virtual {p0, p1}, Lcom/android/internal/telephony/dataconnection/DataConnection;->log(Ljava/lang/String;)V

    .line 450
    return-void

    .line 453
    :cond_49
    iget-object p1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mPhone:Lcom/android/internal/telephony/Phone;

    invoke-virtual {p1}, Lcom/android/internal/telephony/Phone;->getContext()Landroid/content/Context;

    move-result-object p1

    invoke-virtual {p1}, Landroid/content/Context;->getResources()Landroid/content/res/Resources;

    move-result-object p1

    const v0, 0x10e0066

    invoke-virtual {p1, v0}, Landroid/content/res/Resources;->getInteger(I)I

    move-result p1

    .line 455
    if-eqz p1, :cond_73

    .line 456
    invoke-virtual {p2, p1}, Landroid/net/LinkProperties;->setMtu(I)V

    .line 457
    new-instance p2, Ljava/lang/StringBuilder;

    invoke-direct {p2}, Ljava/lang/StringBuilder;-><init>()V

    const-string v0, "MTU set by config resource to: "

    invoke-virtual {p2, v0}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    invoke-virtual {p2}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object p1

    invoke-virtual {p0, p1}, Lcom/android/internal/telephony/dataconnection/DataConnection;->log(Ljava/lang/String;)V

    .line 459
    :cond_73
    return-void

    .line 440
    :cond_74
    :goto_74
    return-void
.end method

.method private clearSettings()V
    .registers 3

    .line 723
    const-string v0, "clearSettings"

    invoke-virtual {p0, v0}, Lcom/android/internal/telephony/dataconnection/DataConnection;->log(Ljava/lang/String;)V

    .line 725
    const-wide/16 v0, -0x1

    iput-wide v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mCreateTime:J

    .line 726
    iput-wide v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mLastFailTime:J

    .line 727
    sget-object v0, Lcom/android/internal/telephony/dataconnection/DcFailCause;->NONE:Lcom/android/internal/telephony/dataconnection/DcFailCause;

    iput-object v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mLastFailCause:Lcom/android/internal/telephony/dataconnection/DcFailCause;

    .line 728
    const/4 v0, -0x1

    iput v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mCid:I

    .line 730
    const/4 v0, 0x5

    new-array v0, v0, [Ljava/lang/String;

    iput-object v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mPcscfAddr:[Ljava/lang/String;

    .line 732
    new-instance v0, Landroid/net/LinkProperties;

    invoke-direct {v0}, Landroid/net/LinkProperties;-><init>()V

    iput-object v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mLinkProperties:Landroid/net/LinkProperties;

    .line 733
    iget-object v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mApnContexts:Ljava/util/HashMap;

    invoke-virtual {v0}, Ljava/util/HashMap;->clear()V

    .line 734
    const/4 v0, 0x0

    iput-object v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mApnSetting:Lcom/android/internal/telephony/dataconnection/ApnSetting;

    .line 736
    return-void
.end method

.method static cmdToString(I)Ljava/lang/String;
    .registers 4

    .param p0, cmd

    .prologue

    .line 260
    const/high16 v0, 0x40000

    sub-int/2addr p0, v0

    .line 261
    if-ltz p0, :cond_f

    sget-object v1, Lcom/android/internal/telephony/dataconnection/DataConnection;->sCmdToString:[Ljava/lang/String;

    array-length v1, v1

    if-ge p0, v1, :cond_f

    .line 262
    sget-object v1, Lcom/android/internal/telephony/dataconnection/DataConnection;->sCmdToString:[Ljava/lang/String;

    aget-object v1, v1, p0

    goto :goto_15

    .line 264
    :cond_f
    add-int v1, p0, v0

    invoke-static {v1}, Lcom/android/internal/telephony/dataconnection/DcAsyncChannel;->cmdToString(I)Ljava/lang/String;

    move-result-object v1

    .line 266
    :goto_15
    if-nez v1, :cond_2d

    .line 267
    new-instance v1, Ljava/lang/StringBuilder;

    invoke-direct {v1}, Ljava/lang/StringBuilder;-><init>()V

    const-string v2, "0x"

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    add-int/2addr p0, v0

    invoke-static {p0}, Ljava/lang/Integer;->toHexString(I)Ljava/lang/String;

    move-result-object p0

    invoke-virtual {v1, p0}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v1

    .line 269
    :cond_2d
    return-object v1
.end method

.method private dumpToLog()V
    .registers 4

    .line 2489
    new-instance v0, Lcom/android/internal/telephony/dataconnection/DataConnection$1;

    new-instance v1, Ljava/io/StringWriter;

    const/4 v2, 0x0

    invoke-direct {v1, v2}, Ljava/io/StringWriter;-><init>(I)V

    invoke-direct {v0, p0, v1}, Lcom/android/internal/telephony/dataconnection/DataConnection$1;-><init>(Lcom/android/internal/telephony/dataconnection/DataConnection;Ljava/io/Writer;)V

    const/4 v1, 0x0

    invoke-virtual {p0, v1, v0, v1}, Lcom/android/internal/telephony/dataconnection/DataConnection;->dump(Ljava/io/FileDescriptor;Ljava/io/PrintWriter;[Ljava/lang/String;)V

    .line 2499
    return-void
.end method

.method private getSuggestedRetryDelay(Landroid/telephony/data/DataCallResponse;)J
    .registers 4

    .line 2331
    invoke-virtual {p1}, Landroid/telephony/data/DataCallResponse;->getSuggestedRetryTime()I

    move-result v0

    if-gez v0, :cond_e

    .line 2332
    const-string p1, "No suggested retry delay."

    invoke-virtual {p0, p1}, Lcom/android/internal/telephony/dataconnection/DataConnection;->log(Ljava/lang/String;)V

    .line 2333
    const-wide/16 v0, -0x2

    return-wide v0

    .line 2336
    :cond_e
    invoke-virtual {p1}, Landroid/telephony/data/DataCallResponse;->getSuggestedRetryTime()I

    move-result v0

    const v1, 0x7fffffff

    if-ne v0, v1, :cond_1f

    .line 2337
    const-string p1, "Modem suggested not retrying."

    invoke-virtual {p0, p1}, Lcom/android/internal/telephony/dataconnection/DataConnection;->log(Ljava/lang/String;)V

    .line 2338
    const-wide/16 v0, -0x1

    return-wide v0

    .line 2343
    :cond_1f
    invoke-virtual {p1}, Landroid/telephony/data/DataCallResponse;->getSuggestedRetryTime()I

    move-result p1

    int-to-long v0, p1

    return-wide v0
.end method

.method private initConnection(Lcom/android/internal/telephony/dataconnection/DataConnection$ConnectionParams;)Z
    .registers 6

    .line 1168
    iget-object v0, p1, Lcom/android/internal/telephony/dataconnection/DataConnection$ConnectionParams;->mApnContext:Lcom/android/internal/telephony/dataconnection/ApnContext;

    .line 1169
    iget-object v1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mApnSetting:Lcom/android/internal/telephony/dataconnection/ApnSetting;

    if-nez v1, :cond_c

    .line 1174
    :cond_c
    iget-object v1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mApnSetting:Lcom/android/internal/telephony/dataconnection/ApnSetting;

    if-eqz v1, :cond_5f

    iget-object v1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mApnSetting:Lcom/android/internal/telephony/dataconnection/ApnSetting;

    invoke-virtual {v0}, Lcom/android/internal/telephony/dataconnection/ApnContext;->getApnType()Ljava/lang/String;

    move-result-object v2

    invoke-virtual {v1, v2}, Lcom/android/internal/telephony/dataconnection/ApnSetting;->canHandleType(Ljava/lang/String;)Z

    move-result v1

    if-nez v1, :cond_1d

    goto :goto_5f

    .line 1181
    :cond_1d
    iget v1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mTag:I

    const/4 v2, 0x1

    add-int/2addr v1, v2

    iput v1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mTag:I

    .line 1182
    iput-object p1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mConnectionParams:Lcom/android/internal/telephony/dataconnection/DataConnection$ConnectionParams;

    .line 1183
    iget-object v1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mConnectionParams:Lcom/android/internal/telephony/dataconnection/DataConnection$ConnectionParams;

    iget v3, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mTag:I

    iput v3, v1, Lcom/android/internal/telephony/dataconnection/DataConnection$ConnectionParams;->mTag:I

    .line 1187
    iget-object v1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mApnContexts:Ljava/util/HashMap;

    invoke-virtual {v1, v0, p1}, Ljava/util/HashMap;->put(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;

    .line 1190
    new-instance p1, Ljava/lang/StringBuilder;

    invoke-direct {p1}, Ljava/lang/StringBuilder;-><init>()V

    const-string v0, "initConnection:  RefCount="

    invoke-virtual {p1, v0}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    iget-object v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mApnContexts:Ljava/util/HashMap;

    .line 1191
    invoke-virtual {v0}, Ljava/util/HashMap;->size()I

    move-result v0

    const-string v0, " mConnectionParams="

    invoke-virtual {p1, v0}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    iget-object v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mConnectionParams:Lcom/android/internal/telephony/dataconnection/DataConnection$ConnectionParams;

    invoke-virtual {p1, v0}, Ljava/lang/StringBuilder;->append(Ljava/lang/Object;)Ljava/lang/StringBuilder;

    invoke-virtual {p1}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object p1

    .line 1190
    invoke-virtual {p0, p1}, Lcom/android/internal/telephony/dataconnection/DataConnection;->log(Ljava/lang/String;)V

    .line 1195
    return v2

    .line 1176
    :cond_5f
    :goto_5f
    new-instance v0, Ljava/lang/StringBuilder;

    invoke-direct {v0}, Ljava/lang/StringBuilder;-><init>()V

    const-string v1, "initConnection: incompatible apnSetting in ConnectionParams cp="

    invoke-virtual {v0, v1}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    invoke-virtual {v0, p1}, Ljava/lang/StringBuilder;->append(Ljava/lang/Object;)Ljava/lang/StringBuilder;

    const-string p1, " dc="

    move-result-object p1

    invoke-virtual {p0, p1}, Lcom/android/internal/telephony/dataconnection/DataConnection;->log(Ljava/lang/String;)V

    .line 1179
    const/4 p1, 0x0

    return p1
.end method

.method private isDnsOk([Ljava/lang/String;)Z
    .registers 6

    .line 780
    const-string v0, "0.0.0.0"

    const/4 v1, 0x0

    aget-object v2, p1, v1

    invoke-virtual {v0, v2}, Ljava/lang/String;->equals(Ljava/lang/Object;)Z

    move-result v0

    const/4 v2, 0x1

    if-eqz v0, :cond_65

    const-string v0, "0.0.0.0"

    aget-object p1, p1, v2

    invoke-virtual {v0, p1}, Ljava/lang/String;->equals(Ljava/lang/Object;)Z

    move-result p1

    if-eqz p1, :cond_65

    iget-object p1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mPhone:Lcom/android/internal/telephony/Phone;

    .line 781
    invoke-virtual {p1}, Lcom/android/internal/telephony/Phone;->isDnsCheckDisabled()Z

    move-result p1

    if-nez p1, :cond_65

    .line 787
    iget-object p1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mApnSetting:Lcom/android/internal/telephony/dataconnection/ApnSetting;

    iget-object p1, p1, Lcom/android/internal/telephony/dataconnection/ApnSetting;->types:[Ljava/lang/String;

    aget-object p1, p1, v1

    const-string v0, "mms"

    invoke-virtual {p1, v0}, Ljava/lang/String;->equals(Ljava/lang/Object;)Z

    move-result p1

    if-eqz p1, :cond_36

    iget-object p1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mApnSetting:Lcom/android/internal/telephony/dataconnection/ApnSetting;

    iget-object p1, p1, Lcom/android/internal/telephony/dataconnection/ApnSetting;->mmsProxy:Ljava/lang/String;

    .line 788
    invoke-static {p1}, Lcom/android/internal/telephony/dataconnection/DataConnection;->isIpAddress(Ljava/lang/String;)Z

    move-result p1

    if-nez p1, :cond_65

    .line 789
    :cond_36
    const-string p1, "isDnsOk: return false apn.types[0]=%s APN_TYPE_MMS=%s isIpAddress(%s)=%s"

    const/4 v0, 0x4

    new-array v0, v0, [Ljava/lang/Object;

    iget-object v3, v3, Lcom/android/internal/telephony/dataconnection/ApnSetting;->types:[Ljava/lang/String;

    aget-object v3, v3, v1

    aput-object v3, v0, v1

    const-string v3, "mms"

    aput-object v3, v0, v2

    const/4 v2, 0x2

    .line 792
    invoke-static {v3}, Lcom/android/internal/telephony/dataconnection/DataConnection;->isIpAddress(Ljava/lang/String;)Z

    move-result v3

    invoke-static {v3}, Ljava/lang/Boolean;->valueOf(Z)Ljava/lang/Boolean;

    move-result-object v3

    aput-object v3, v0, v2

    .line 789
    invoke-static {p1, v0}, Ljava/lang/String;->format(Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;

    move-result-object p1

    invoke-virtual {p0, p1}, Lcom/android/internal/telephony/dataconnection/DataConnection;->log(Ljava/lang/String;)V

    .line 793
    return v1

    .line 796
    :cond_65
    return v2
.end method

.method private setNetworkRestriction()V
    .registers 5

    .line 914
    const/4 v0, 0x0

    iput-boolean v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mRestrictedNetworkOverride:Z

    .line 916
    nop

    .line 917
    iget-object v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mApnContexts:Ljava/util/HashMap;

    invoke-virtual {v0}, Ljava/util/HashMap;->keySet()Ljava/util/Set;

    move-result-object v0

    invoke-interface {v0}, Ljava/util/Set;->iterator()Ljava/util/Iterator;

    move-result-object v0

    const/4 v1, 0x1

    move v2, v1

    :goto_10
    invoke-interface {v0}, Ljava/util/Iterator;->hasNext()Z

    move-result v3

    if-eqz v3, :cond_22

    invoke-interface {v0}, Ljava/util/Iterator;->next()Ljava/lang/Object;

    move-result-object v3

    check-cast v3, Lcom/android/internal/telephony/dataconnection/ApnContext;

    .line 918
    invoke-virtual {v3, v1}, Lcom/android/internal/telephony/dataconnection/ApnContext;->hasNoRestrictedRequests(Z)Z

    move-result v3

    and-int/2addr v2, v3

    .line 919
    goto :goto_10

    .line 920
    :cond_22
    if-eqz v2, :cond_25

    .line 921
    return-void

    .line 926
    :cond_25
    iget-object v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mApnSetting:Lcom/android/internal/telephony/dataconnection/ApnSetting;

    iget-object v2, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mPhone:Lcom/android/internal/telephony/Phone;

    invoke-virtual {v0, v2}, Lcom/android/internal/telephony/dataconnection/ApnSetting;->isMetered(Lcom/android/internal/telephony/Phone;)Z

    move-result v0

    if-nez v0, :cond_30

    .line 927
    return-void

    .line 931
    :cond_30
    iget-object v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mDct:LDataconnection/DcTracker;

    invoke-virtual {v0}, Lcom/android/internal/telephony/dataconnection/DcTracker;->isDataEnabled()Z

    move-result v0

    xor-int/2addr v0, v1

    iput-boolean v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mRestrictedNetworkOverride:Z

    .line 932
    return-void
.end method

.method static slog(LData/String;)V
    .registers 2

    .line 2396
    const-string v0, "DC"

    invoke-static {v0, p0}, Landroid/telephony/Rlog;->d(Ljava/lang/String;Ljava/lang/String;)I

    .line 2397
    return-void
.end method

.method private updateTcpBufferSizes(I)V
    .registers 10

    .line 812
    nop

    .line 813
    const/16 v0, 0x13

    if-ne p1, v0, :cond_7

    .line 816
    const/16 p1, 0xe

    .line 818
    :cond_7
    invoke-static {p1}, Landroid/telephony/ServiceState;->rilRadioTechnologyToString(I)Ljava/lang/String;

    move-result-object v1

    sget-object v2, Ljava/util/Locale;->ROOT:Ljava/util/Locale;

    invoke-virtual {v1, v2}, Ljava/lang/String;->toLowerCase(Ljava/util/Locale;)Ljava/lang/String;

    move-result-object v1

    .line 821
    const/4 v2, 0x7

    if-eq p1, v2, :cond_1c

    const/16 v2, 0x8

    if-eq p1, v2, :cond_1c

    const/16 v2, 0xc

    if-ne p1, v2, :cond_1e

    .line 824
    :cond_1c
    const-string v1, "evdo"

    .line 828
    :cond_1e
    iget-object v2, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mPhone:Lcom/android/internal/telephony/Phone;

    invoke-virtual {v2}, Lcom/android/internal/telephony/Phone;->getContext()Landroid/content/Context;

    move-result-object v2

    invoke-virtual {v2}, Landroid/content/Context;->getResources()Landroid/content/res/Resources;

    move-result-object v2

    const v3, 0x1070039

    invoke-virtual {v2, v3}, Landroid/content/res/Resources;->getStringArray(I)[Ljava/lang/String;

    move-result-object v2

    .line 830
    const/4 v3, 0x0

    move v4, v3

    :goto_31
    array-length v5, v2

    if-ge v4, v5, :cond_4f

    .line 831
    aget-object v5, v2, v4

    const-string v6, ":"

    invoke-virtual {v5, v6}, Ljava/lang/String;->split(Ljava/lang/String;)[Ljava/lang/String;

    move-result-object v5

    .line 832
    aget-object v6, v5, v3

    invoke-virtual {v1, v6}, Ljava/lang/String;->equals(Ljava/lang/Object;)Z

    move-result v6

    if-eqz v6, :cond_4c

    array-length v6, v5

    const/4 v7, 0x2

    if-ne v6, v7, :cond_4c

    .line 833
    const/4 v1, 0x1

    aget-object v1, v5, v1

    .line 834
    goto :goto_50

    .line 830
    :cond_4c
    add-int/lit8 v4, v4, 0x1

    goto :goto_31

    .line 838
    :cond_4f
    const/4 v1, 0x0

    :goto_50
    if-nez v1, :cond_79

    .line 842
    if-eq p1, v0, :cond_76

    packed-switch p1, :pswitch_data_80

    packed-switch p1, :pswitch_data_8a

    goto :goto_79

    .line 875
    :pswitch_5b
    const-string v1, "122334,734003,2202010,32040,192239,576717"

    .line 845
    goto :goto_79

    .line 872
    :cond_76
    :pswitch_76
    const-string v1, "524288,1048576,2097152,262144,524288,1048576"

    .line 873
    nop

    .line 882
    :cond_79
    :goto_79
    iget-object p1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mLinkProperties:Landroid/net/LinkProperties;

    invoke-virtual {p1, v1}, Landroid/net/LinkProperties;->setTcpBufferSizes(Ljava/lang/String;)V

    .line 883
    return-void

    nop

    :pswitch_data_80
    .packed-switch 0x1
        :pswitch_73
        :pswitch_70
        :pswitch_6d
    .end packed-switch

    :pswitch_data_8a
    .packed-switch 0x6
        :pswitch_6a
        :pswitch_67
        :pswitch_67
        :pswitch_64
        :pswitch_61
        :pswitch_61
        :pswitch_67
        :pswitch_5e
        :pswitch_76
        :pswitch_5b
    .end packed-switch
.end method


# virtual methods
.method dispose()V
    .registers 2

    .line 292
    const-string v0, "dispose: call quiteNow()"

    invoke-virtual {p0, v0}, Lcom/android/internal/telephony/dataconnection/DataConnection;->log(Ljava/lang/String;)V

    .line 293
    invoke-virtual {p0}, Lcom/android/internal/telephony/dataconnection/DataConnection;->quitNow()V

    .line 294
    return-void
.end method

.method public dump(Ljava/io/FileDescriptor;Ljava/io/PrintWriter;[Ljava/lang/String;)V
    .registers 7

    .line 2510
    new-instance v0, Lcom/android/internal/util/IndentingPrintWriter;

    const-string v1, " "

    invoke-direct {v0, p2, v1}, Lcom/android/internal/util/IndentingPrintWriter;-><init>(Ljava/io/Writer;Ljava/lang/String;)V

    .line 2543
    invoke-virtual {v0}, Lcom/android/internal/util/IndentingPrintWriter;->flush()V

    .line 2544
    return-void
.end method

.method public getDataConnectionId()I
    .registers 2

    .line 710
    iget v0, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mId:I

    return v0
.end method

.method getNetworkCapabilities()Landroid/net/NetworkCapabilities;
    .registers 16

    .line 935
    new-instance v0, Landroid/net/NetworkCapabilities;

    invoke-direct {v0}, Landroid/net/NetworkCapabilities;-><init>()V

    .line 936
    const/4 v1, 0x0

    invoke-virtual {v0, v1}, Landroid/net/NetworkCapabilities;->addTransportType(I)Landroid/net/NetworkCapabilities;

    .line 938
    iget-object v2, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mApnSetting:Lcom/android/internal/telephony/dataconnection/ApnSetting;

    const/16 v3, 0xb

    const/4 v4, 0x2

    const/4 v5, 0x1

    :sswitch_b7
    const-string v10, "*"

    invoke-virtual {v8, v10}, Ljava/lang/String;->equals(Ljava/lang/Object;)Z

    move-result v8

    if-eqz v8, :cond_c1

    move v8, v1

    goto :goto_c2

    :cond_c1
    :goto_c1
    move v8, v9

    :goto_c2
    const/16 v9, 0xc

    packed-switch v8, :pswitch_data_1f6

    goto :goto_107

    .line 964
    goto :goto_107

    .line 959
    :pswitch_ea
    invoke-virtual {v0, v9}, Landroid/net/NetworkCapabilities;->addCapability(I)Landroid/net/NetworkCapabilities;

    .line 960
    goto :goto_107

    .line 948
    :pswitch_ee
    invoke-virtual {v0, v9}, Landroid/net/NetworkCapabilities;->addCapability(I)Landroid/net/NetworkCapabilities;

    .line 956
    nop

    .line 939
    :goto_107
    add-int/lit8 v7, v7, 0x1

    goto/16 :goto_17

    .line 1001
    :cond_10b
    iget-object v1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mConnectionParams:Lcom/android/internal/telephony/dataconnection/DataConnection$ConnectionParams;

    if-eqz v1, :cond_119

    iget-object v1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mConnectionParams:Lcom/android/internal/telephony/dataconnection/DataConnection$ConnectionParams;

    iget-boolean v1, v1, Lcom/android/internal/telephony/dataconnection/DataConnection$ConnectionParams;->mUnmeteredUseOnly:Z

    if-eqz v1, :cond_119

    iget-boolean v1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mRestrictedNetworkOverride:Z

    if-eqz v1, :cond_123

    :cond_119
    iget-object v1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mApnSetting:Lcom/android/internal/telephony/dataconnection/ApnSetting;

    iget-object v2, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mPhone:Lcom/android/internal/telephony/Phone;

    .line 1003
    invoke-virtual {v1, v2}, Lcom/android/internal/telephony/dataconnection/ApnSetting;->isMetered(Lcom/android/internal/telephony/Phone;)Z

    move-result v1

    if-nez v1, :cond_127

    .line 1004
    :cond_123
    invoke-virtual {v0, v3}, Landroid/net/NetworkCapabilities;->addCapability(I)Landroid/net/NetworkCapabilities;

    goto :goto_12a

    .line 1006
    :cond_127
    invoke-virtual {v0, v3}, Landroid/net/NetworkCapabilities;->removeCapability(I)Landroid/net/NetworkCapabilities;

    .line 1009
    :goto_12a
    invoke-virtual {v0}, Landroid/net/NetworkCapabilities;->maybeMarkCapabilitiesRestricted()V

    .line 1011
    :cond_12d
    iget-boolean v1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mRestrictedNetworkOverride:Z

    if-eqz v1, :cond_139

    .line 1012
    const/16 v1, 0xd

    invoke-virtual {v0, v1}, Landroid/net/NetworkCapabilities;->removeCapability(I)Landroid/net/NetworkCapabilities;

    .line 1014
    invoke-virtual {v0, v4}, Landroid/net/NetworkCapabilities;->removeCapability(I)Landroid/net/NetworkCapabilities;

    .line 1017
    :cond_139
    nop

    .line 1018
    nop

    .line 1019
    iget v1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mRilRat:I

    const/16 v2, 0x13

    const/16 v6, 0x733

    const/16 v7, 0x99

    const v8, 0x19000

    const v9, 0xc800

    const/16 v10, 0x3800

    const/16 v11, 0xe

    if-eq v1, v2, :cond_18a

    packed-switch v1, :pswitch_data_20e

    .line 1038
    :goto_152
    move v6, v11

    move v8, v6

    goto :goto_18b

    .line 1035
    :pswitch_155
    const/16 v6, 0x2c00

    const v8, 0xa800

    goto :goto_18b

    .line 1032
    :pswitch_15b
    nop

    .line 1038
    :goto_15c
    move v6, v9

    goto :goto_18b

    .line 1033
    :cond_18a
    goto :goto_15c

    .line 1038
    :goto_18b
    invoke-virtual {v0, v6}, Landroid/net/NetworkCapabilities;->setLinkUpstreamBandwidthKbps(I)Landroid/net/NetworkCapabilities;

    .line 1039
    invoke-virtual {v0, v8}, Landroid/net/NetworkCapabilities;->setLinkDownstreamBandwidthKbps(I)Landroid/net/NetworkCapabilities;

    .line 1041
    new-instance v1, Landroid/net/StringNetworkSpecifier;

    iget-object v2, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mPhone:Lcom/android/internal/telephony/Phone;

    invoke-virtual {v2}, Lcom/android/internal/telephony/Phone;->getSubId()I

    move-result v2

    invoke-static {v2}, Ljava/lang/Integer;->toString(I)Ljava/lang/String;

    move-result-object v2

    invoke-direct {v1, v2}, Landroid/net/StringNetworkSpecifier;-><init>(Ljava/lang/String;)V

    invoke-virtual {v0, v1}, Landroid/net/NetworkCapabilities;->setNetworkSpecifier(Landroid/net/NetworkSpecifier;)Landroid/net/NetworkCapabilities;

    .line 1043
    const/16 v1, 0x12

    iget-object v2, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mPhone:Lcom/android/internal/telephony/Phone;

    .line 1044
    invoke-virtual {v2}, Lcom/android/internal/telephony/Phone;->getServiceState()Landroid/telephony/ServiceState;

    move-result-object v2

    invoke-virtual {v2}, Landroid/telephony/ServiceState;->getDataRoaming()Z

    move-result v2

    .line 1043
    xor-int/2addr v2, v5

    invoke-virtual {v0, v1, v2}, Landroid/net/NetworkCapabilities;->setCapability(IZ)Landroid/net/NetworkCapabilities;

    .line 1046
    const/16 v1, 0x14

    invoke-virtual {v0, v1}, Landroid/net/NetworkCapabilities;->addCapability(I)Landroid/net/NetworkCapabilities;

    .line 1049
    iget v1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mSubscriptionOverride:I

    and-int/2addr v1, v5

    if-eqz v1, :cond_1c0

    .line 1050
    invoke-virtual {v0, v3}, Landroid/net/NetworkCapabilities;->addCapability(I)Landroid/net/NetworkCapabilities;

    .line 1052
    :cond_1c0
    iget v1, p0, Lcom/android/internal/telephony/dataconnection/DataConnection;->mSubscriptionOverride:I

    and-int/2addr v1, v4

    if-eqz v1, :cond_1ca

    .line 1053
    const/16 v1, 0x14

    invoke-virtual {v0, v1}, Landroid/net/NetworkCapabilities;->removeCapability(I)Landroid/net/NetworkCapabilities;

    .line 1056
    :cond_1ca
    return-object v0

    nop

    :sswitch_data_1cc
    .sparse-switch
        0x2a -> :sswitch_b7
        0xd18 -> :sswitch_ac
        0x17ff4 -> :sswitch_a2
        0x185fd -> :sswitch_98
        0x197cf -> :sswitch_8e
        0x1a6d3 -> :sswitch_84
        0x300cf6 -> :sswitch_7a
        0x360bde -> :sswitch_70
        0x5c13d641 -> :sswitch_66
        0x6118c591 -> :sswitch_5a
    .end sparse-switch

    :pswitch_data_1f6
    .packed-switch 0x0
        :pswitch_ee
        :pswitch_ea
        :pswitch_e6
        :pswitch_d2
        :pswitch_ce
        :pswitch_c8
    .end packed-switch

    :pswitch_data_20e
    .packed-switch 0x1
        :pswitch_185
        :pswitch_180
        :pswitch_17b
        :pswitch_17a
        :pswitch_164
        :pswitch_161
        :pswitch_15e
        :pswitch_15b
        :pswitch_155
    .end packed-switch
.end method
