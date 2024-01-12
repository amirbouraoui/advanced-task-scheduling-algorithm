package schedule;

import java.util.ArrayList;
import java.util.List;

import com.flexganttfx.model.Row;

public class Machine extends Row<Machine, Machine, Task> {
    private List<TaskInstance> taskInstances;

    public Machine(String name) {
        super(name);
        taskInstances = new ArrayList<>();
    }

    public void addTaskInstance(TaskInstance taskInstance) {
        taskInstances.add(taskInstance);
    }

    public List<TaskInstance> getTaskInstances() {
        return taskInstances;
    }
}
