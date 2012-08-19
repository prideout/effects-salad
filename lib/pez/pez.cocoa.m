#import <OpenGL/OpenGL.h>
#import <OpenGL/GL.h>
#import "pez.h"
#import "bstrlib.h"
#import <Cocoa/Cocoa.h>
#import <mach/mach_time.h>
#import <wchar.h>
#import <stdio.h>

@class View;

@interface View : NSOpenGLView <NSWindowDelegate> {
    NSRect m_frameRect;
    BOOL m_didInit;
    uint64_t m_previousTime;
    NSTimer* m_timer;
}

- (void) animate;

@end

@implementation View

-(void)windowWillClose:(NSNotification *)note {
    [[NSApplication sharedApplication] terminate:self];
}

- (void) timerFired:(NSTimer*) timer
{
    [self animate];     
}

- (id) initWithFrame: (NSRect) frame
{
    m_didInit = FALSE;
    
    int attribs[] = {
        NSOpenGLPFAAccelerated,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFAColorSize, 32,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
        NSOpenGLPFANoRecovery,
        kCGLPFASampleBuffers, 1,
        kCGLPFASamples, 8,
        0
    };

    if (!PezGetConfig().Multisampling) {
        attribs[11] = 0;
    }

    NSOpenGLPixelFormat *fmt = [[NSOpenGLPixelFormat alloc]
                            initWithAttributes:(NSOpenGLPixelFormatAttribute*) attribs];

    self = [self initWithFrame:frame pixelFormat:fmt];

    [fmt release];

    m_frameRect = frame;
    m_previousTime = mach_absolute_time();

    m_timer = [[NSTimer
                       scheduledTimerWithTimeInterval:1.0f/120.0f
                       target:self 
                       selector:@selector(timerFired:)
                       userInfo:nil
                       repeats:YES] retain];

    return self;
}

- (void) drawRect:(NSRect) theRect
{
    if (!m_didInit) {
            
        int TransparentWindow = 0;
        if (TransparentWindow) {
            int opaque = NO;
            [[self openGLContext]
                setValues:&opaque
                forParameter:NSOpenGLCPSurfaceOpacity];
    
            [[self window] setOpaque:NO];
            [[self window] setAlphaValue:0.99];
        }

        // Reset OpenGL error state:
        glGetError();
    
        // Lop off the trailing .c
        bstring name = bfromcstr(PezGetConfig().Title);
        bstring shaderPrefix = bfromcstr("shaders/"); //bmidstr(name, 0, blength(name) - 1);
        pezSwInit(bdata(shaderPrefix));
        bdestroy(shaderPrefix);

        // Set up the Shader Wrangler
        //pezSwAddPath("./", ".glsl");
        //pezSwAddPath("../", ".glsl");
        pezSwAddDirective("*", "#version 150");

        // Perform user-specified intialization
        pezPrintString("OpenGL Version: %s\n", glGetString(GL_VERSION));
        PezInitialize();

        bstring windowTitle = bmidstr(name, 0, blength(name) - 2);

        m_didInit = YES;
        
        [[self window] setLevel: NSFloatingWindowLevel];
        [[self window] makeKeyAndOrderFront: self];
        [[self window] setTitle: [NSString stringWithUTF8String: bdata(windowTitle)]];
    }

    PezRender();
    
    if (glGetError() != GL_NO_ERROR) {
        pezFatal("OpenGL Error\n");
    }
    
    [[self openGLContext] flushBuffer]; 
}

- (void) mouseDragged: (NSEvent*) theEvent
{
    NSPoint curPoint = [theEvent locationInWindow];
    PezHandleMouse(curPoint.x, m_frameRect.size.height - curPoint.y, PEZ_MOVE);
}

- (void) mouseUp: (NSEvent*) theEvent
{
    NSPoint curPoint = [theEvent locationInWindow];
    PezHandleMouse(curPoint.x, m_frameRect.size.height - curPoint.y, PEZ_UP);
}

- (void) mouseDown: (NSEvent*) theEvent
{
    NSPoint curPoint = [theEvent locationInWindow];
    PezHandleMouse(curPoint.x, m_frameRect.size.height - curPoint.y, PEZ_DOWN);
}

- (void) animate
{
    uint64_t currentTime = mach_absolute_time();
    uint64_t elapsedTime = currentTime - m_previousTime;
    m_previousTime = currentTime;
    
    mach_timebase_info_data_t info;
    mach_timebase_info(&info);
    
    elapsedTime *= info.numer;
    elapsedTime /= info.denom;
    
    float timeStep = elapsedTime / 1000000.0f;

    PezUpdate(timeStep / 1000.0f);
    
    [self display];
}

- (void) onKey: (unichar) character downEvent: (BOOL) flag
{
    switch(character) {
        case 27:
        case 'q':
            [[NSApplication sharedApplication] terminate:self];
            break;
    }
}

- (void) keyDown:(NSEvent *)theEvent
{
    NSString *characters;
    unsigned int characterIndex, characterCount;
    
    characters = [theEvent charactersIgnoringModifiers];
    characterCount = [characters length];

    for (characterIndex = 0; characterIndex < characterCount; characterIndex++)
        [self onKey:[characters characterAtIndex:characterIndex] downEvent:YES];
}

@end

int main(int argc, const char *argv[])
{
    NSAutoreleasePool *pool = [NSAutoreleasePool new];
    NSApplication *NSApp = [NSApplication sharedApplication];
    
    NSRect screenBounds = [[NSScreen mainScreen] frame];
    NSRect viewBounds = NSMakeRect(0, 0, PezGetConfig().Width, PezGetConfig().Height);
    
    View* view = [[View alloc] initWithFrame:viewBounds];
    
    NSRect centered = NSMakeRect(NSMidX(screenBounds) - NSMidX(viewBounds),
                                 NSMidY(screenBounds) - NSMidY(viewBounds),
                                 viewBounds.size.width, viewBounds.size.height);
    
    NSWindow *window = [[NSWindow alloc]
        initWithContentRect:centered
        styleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask
        backing:NSBackingStoreBuffered
        defer:NO];

    [window setContentView:view];
    [window setDelegate:view];
    [window makeKeyAndOrderFront:nil];

    [view release];
    
    [NSApp run];
    
    [pool release];
    return EXIT_SUCCESS;
}

void pezPrintStringW(const wchar_t* pStr, ...)
{
    va_list a;
    va_start(a, pStr);

    wchar_t msg[1024] = {0};
    vswprintf(msg, countof(msg), pStr, a);
    fputws(msg, stderr);
}

void pezPrintString(const char* pStr, ...)
{
    va_list a;
    va_start(a, pStr);

    char msg[1024] = {0};
    vsnprintf(msg, countof(msg), pStr, a);
    fputs(msg, stderr);
}

void _pezFatalW(const wchar_t* pStr, va_list a)
{
    wchar_t msg[1024] = {0};
    vswprintf(msg, countof(msg), pStr, a);
    fputws(msg, stderr);
    exit(1);
}

void _pezFatal(const char* pStr, va_list a)
{
    char msg[1024] = {0};
    vsnprintf(msg, countof(msg), pStr, a);
    puts(msg);
    puts("\n");
    exit(1);
}

void pezFatal(const char* pStr, ...)
{
    va_list a;
    va_start(a, pStr);
    _pezFatal(pStr, a);
}

void pezFatalW(const wchar_t* pStr, ...)
{
    va_list a;
    va_start(a, pStr);
    _pezFatalW(pStr, a);
}

void pezCheck(int condition, ...)
{
    va_list a;
    const char* pStr;
    
    if (condition)
        return;
    
    va_start(a, condition);
    pStr = va_arg(a, const char*);
    _pezFatal(pStr, a);
}

void pezCheckW(int condition, ...)
{
    va_list a;
    const wchar_t* pStr;
    
    if (condition)
        return;
    
    va_start(a, condition);
    pStr = va_arg(a, const wchar_t*);
    _pezFatalW(pStr, a);
}
