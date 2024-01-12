package application;

public class Scheduling {
    static {
        System.loadLibrary("scheduling");
    }

    private final int returnValue;

    public native int runSchedule(String instanceInFile, String scheduleOutFile, int m, int preemption, int balanced);

    public Scheduling(String instanceInFile, String scheduleOutFile, int machines, int preemption, int balanced) {
        returnValue = runSchedule(instanceInFile, scheduleOutFile, machines, preemption, balanced);
    }

    /**
     * Returns the status of the scheduling operation.
     * 
     * The return value signifies whether the scheduling operation was successful or not.
     * When the return value is 0, it means that the scheduling operation was successful 
     * and a notification indicating that the task has been uploaded is displayed.
     * Any non-zero value indicates an error in the scheduling operation.
     * 
     */
    public int getReturnValue() {
        return returnValue;
    }
}
