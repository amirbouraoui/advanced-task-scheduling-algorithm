package schedule;

public class TaskInstance {
    private int startTime;
    private int endTime;
    private String taskId;

    public TaskInstance(int startTime, int endTime, String taskId) {
        this.startTime = startTime;
        this.endTime = endTime;
        this.taskId = taskId;
    }

    public int getStartTime() {
        return startTime;
    }

    public void setStartTime(int startTime) {
        this.startTime = startTime;
    }

    public int getEndTime() {
        return endTime;
    }

    public void setEndTime(int endTime) {
        this.endTime = endTime;
    }

    public String getTaskId() {
        return taskId;
    }

    public void setTaskId(String taskId) {
        this.taskId = taskId;
    }

    @Override
    public String toString() {
        return startTime + " " + endTime + " " + taskId;
    }
}
