import org.lwjgl.system.MemoryStack;
import org.lwjgl.PointerBuffer;
import org.lwjgl.vulkan.*;
import java.util.*;
import java.nio.*;

import static org.lwjgl.vulkan.VK10.*;

public class intelBoot {
    interface IntelCommand {
        void run(String[] args, Object module);
    }

    static class IntelModuleRegistry {
        private static final Map<String, Object> modules = new HashMap<>();
        public static void register(String name, Object module) {
            modules.put(name, module);
        }
        public static Object get(String name) {
            return modules.get(name);
        }
    }

    static class IntelCommandParser {
        private static final Map<String, Map<String, IntelCommand>> registry = new HashMap<>();
        public static void register(String module, String cmd, IntelCommand handler) {
            registry.computeIfAbsent(module, k -> new HashMap<>()).put(cmd, handler);
        }
        public static void execute(String input) {
            String[] tokens = input.trim().split("\\s+");
            if (tokens.length < 2) {
                System.out.println("[intelERR] Usage: <module> <command> [args]");
                return;
            }
            String module = tokens[0], command = tokens[1];
            String[] args = Arrays.copyOfRange(tokens, 2, tokens.length);
            Object modObj = IntelModuleRegistry.get(module);
            IntelCommand cmdObj = registry.getOrDefault(module, Map.of()).get(command);
            if (cmdObj != null && modObj != null) {
                cmdObj.run(args, modObj);
            } else {
                System.out.println("[intelERR] Unknown command or module.");
            }
        }
    }

    // ── GFX: Adapter Info ────────────────────────────────────────────────────────
    static class GfxAdapterCommand implements IntelCommand {
        @Override public void run(String[] args, Object module) {
            try (MemoryStack stack = MemoryStack.stackPush()) {
                VkInstanceCreateInfo ci = VkInstanceCreateInfo.calloc(stack)
                    .sType(VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO);

                PointerBuffer pInst = stack.mallocPointer(1);
                if (vkCreateInstance(ci, null, pInst) != VK_SUCCESS) {
                    System.out.println("[gfx] Vulkan init failed.");
                    return;
                }
                VkInstance instance = new VkInstance(pInst.get(0), ci);

                IntBuffer count = stack.ints(0);
                vkEnumeratePhysicalDevices(instance, count, null);
                if (count.get(0) == 0) {
                    System.out.println("[gfx] No Vulkan devices found.");
                    vkDestroyInstance(instance, null);
                    return;
                }

                PointerBuffer devices = stack.mallocPointer(count.get(0));
                vkEnumeratePhysicalDevices(instance, count, devices);
                VkPhysicalDevice phyDev = new VkPhysicalDevice(devices.get(0), instance);

                VkPhysicalDeviceProperties props = VkPhysicalDeviceProperties.calloc(stack);
                vkGetPhysicalDeviceProperties(phyDev, props);

                int api = props.apiVersion();
                String name = props.deviceNameString();

                System.out.println("[gfx] Adapter Name  : " + name);
                System.out.printf("[gfx] Vulkan Ver    : %d.%d.%d%n",
                    VK_VERSION_MAJOR(api), VK_VERSION_MINOR(api), VK_VERSION_PATCH(api));

                vkDestroyInstance(instance, null);
            } catch (Exception e) {
                System.out.println("[gfx] Adapter check failed: " + e.getMessage());
            }
        }
    }

    // ── GFX: Real FPS Benchmark ───────────────────────────────────────────────────
    static class GfxFpsCommand implements IntelCommand {
@Override public void run(String[] args, Object module) {
    int durationSec = 5;
    System.out.println("[gfx] Running Vulkan FPS benchmark (" + durationSec + " sec)...");

    try (MemoryStack stack = MemoryStack.stackPush()) {
        VkInstanceCreateInfo ci = VkInstanceCreateInfo.calloc(stack)
            .sType(VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO);

        PointerBuffer pInst = stack.mallocPointer(1);
        if (vkCreateInstance(ci, null, pInst) != VK_SUCCESS) {
            System.out.println("[gfx] Vulkan init failed.");
            return;
        }
        VkInstance instance = new VkInstance(pInst.get(0), ci);

        IntBuffer count = stack.ints(0);
        vkEnumeratePhysicalDevices(instance, count, null);
        if (count.get(0) == 0) {
            System.out.println("[gfx] No Vulkan GPU found.");
            vkDestroyInstance(instance, null);
            return;
        }

        PointerBuffer devices = stack.mallocPointer(count.get(0));
        vkEnumeratePhysicalDevices(instance, count, devices);
        VkPhysicalDevice phyDev = new VkPhysicalDevice(devices.get(0), instance);

        VkPhysicalDeviceProperties props = VkPhysicalDeviceProperties.calloc(stack);
        vkGetPhysicalDeviceProperties(phyDev, props);
        String name = props.deviceNameString();
        int api = props.apiVersion();

        // ── Queue Family Selection ──
        IntBuffer queueCount = stack.ints(0);
        vkGetPhysicalDeviceQueueFamilyProperties(phyDev, queueCount, null);
        int qCount = queueCount.get(0);
        VkQueueFamilyProperties.Buffer queueProps = VkQueueFamilyProperties.calloc(qCount, stack);
        vkGetPhysicalDeviceQueueFamilyProperties(phyDev, queueCount, queueProps);

        int graphicsQueueIndex = -1;
        for (int i = 0; i < qCount; i++) {
            if ((queueProps.get(i).queueFlags() & VK_QUEUE_GRAPHICS_BIT) != 0) {
                graphicsQueueIndex = i;
                break;
            }
        }
        if (graphicsQueueIndex == -1) {
            System.out.println("[gfx] No graphics queue found.");
            vkDestroyInstance(instance, null);
            return;
        }

        FloatBuffer priorities = stack.floats(1.0f);
        VkDeviceQueueCreateInfo.Buffer queueInfo = VkDeviceQueueCreateInfo.calloc(1, stack)
            .sType(VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO)
            .queueFamilyIndex(graphicsQueueIndex)
            .pQueuePriorities(priorities);

        VkDeviceCreateInfo dci = VkDeviceCreateInfo.calloc(stack)
            .sType(VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO)
            .pQueueCreateInfos(queueInfo);

        PointerBuffer pDev = stack.mallocPointer(1);
        if (vkCreateDevice(phyDev, dci, null, pDev) != VK_SUCCESS) {
            System.out.println("[gfx] Failed to create logical device.");
            vkDestroyInstance(instance, null);
            return;
        }
        VkDevice device = new VkDevice(pDev.get(0), phyDev, dci);

        // ── Benchmark Ritual ──
long start = System.nanoTime();
int frames = 0;
long frameDuration = 1_000_000_000L / 240; // nanoseconds per frame

while ((System.nanoTime() - start) < durationSec * 1_000_000_000L) {
    long frameStart = System.nanoTime();

    frames++; // simulate frame render

    long frameEnd = System.nanoTime();
    long timeTaken = frameEnd - frameStart;
    long sleepTime = frameDuration - timeTaken;

    if (sleepTime > 0) {
        try {
            Thread.sleep(sleepTime / 1_000_000L, (int)(sleepTime % 1_000_000L));
        } catch (InterruptedException e) {
            // optional: log as ritual interruption
        }
    }
}
        long end = System.nanoTime();
        double elapsed = (end - start) / 1_000_000_000.0;
        double fps = frames / elapsed;

        System.out.println("[gfx] Adapter        : " + name);
        System.out.printf("[gfx] API Version    : Vulkan %d.%d%n",
            VK_VERSION_MAJOR(api), VK_VERSION_MINOR(api));
        System.out.printf("[gfx] Frames Rendered: %d%n", frames);
        System.out.printf("[gfx] Time Elapsed   : %.2f sec%n", elapsed);
        System.out.printf("[gfx] Average FPS    : %.2f%n", fps);
        System.out.println("[gfx] Render Tier    : " + tier(fps));

        vkDestroyDevice(device, null);
        vkDestroyInstance(instance, null);
    } catch (Exception e) {
        System.out.println("[gfx] Benchmark error: " + e.getMessage());
    }
}

        private String tier(double fps) {
            if (fps >= 2000) return "very high";
            if (fps >= 1000) return "high";
            if (fps >= 500)  return "medium";
            if (fps >= 250)  return "almost low";
            return "very low i guess";
        }
    }

    // ── SYS: Vulkan Binding & Native Check ────────────────────────────────────────
    static class SysCheckCommand implements IntelCommand {
        @Override public void run(String[] args, Object module) {
            System.out.println("[sys] Running LWJGL Vulkan diagnostics...");
            try {
                Class.forName("org.lwjgl.vulkan.VK10");
                Class.forName("org.lwjgl.system.MemoryStack");
                System.out.println("[sys] Vulkan bindings: available");
            } catch (Exception e) {
                System.out.println("[sys] Vulkan bindings: missing");
            }
            try {
                System.loadLibrary("lwjgl");
                System.out.println("[sys] Native lib 'lwjgl': loaded");
            } catch (UnsatisfiedLinkError ule) {
                System.out.println("[sys] Native lib 'lwjgl': not found");
            }
        }
    }

    // ── SYS: Basic System Info ─────────────────────────────────────────────────────
    static class SysInfoCommand implements IntelCommand {
        @Override public void run(String[] args, Object module) {
            System.out.println("[sys] Environment Info:");
            System.out.println("      OS Name    : " + System.getProperty("os.name"));
            System.out.println("      OS Arch    : " + System.getProperty("os.arch"));
            System.out.println("      Java Ver   : " + System.getProperty("java.version"));
            System.out.println("      CPU Cores  : " + Runtime.getRuntime().availableProcessors());
        }
    }

    public static void main(String[] args) {
        IntelModuleRegistry.register("gfx", new Object());
        IntelCommandParser.register("gfx", "adapter", new GfxAdapterCommand());
        IntelCommandParser.register("gfx", "fps",     new GfxFpsCommand());

        IntelModuleRegistry.register("sys", new Object());
        IntelCommandParser.register("sys", "check",   new SysCheckCommand());
        IntelCommandParser.register("sys", "info",    new SysInfoCommand());

        System.out.println("intelOS :: Boot Console Online. Type 'exit' to quit.");
        new SysCheckCommand().run(null, null);
        new SysInfoCommand().run(null, null);

        Scanner sc = new Scanner(System.in);
        while (true) {
            System.out.print("[intel] > ");
            String input = sc.nextLine().trim();
            if (input.equalsIgnoreCase("exit")) break;
            IntelCommandParser.execute(input);
        }
        System.out.println("intelOS :: Shutdown complete.");
    }
}
